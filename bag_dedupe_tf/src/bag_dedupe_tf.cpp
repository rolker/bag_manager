#include <rosbag/bag.h>
#include <rosbag/view.h>
#include "progressbar.hpp"
#include <iostream>
#include <tf2_msgs/TFMessage.h>

void usage()
{ 
  std::cout << "usage: bag_dedupe_tf [options and input files]\n";
  std::cout << "-c, --compression Compression format: none, lz4 or bz2 (default lz4)\n";
  std::cout << "-h, Display this message\n";
  std::cout << "-p, Display progress\n";
  exit(-1);
}

int main(int argc, char *argv[])
{
//################ Defaults ###################

  rosbag::compression::CompressionType compression = rosbag::compression::LZ4;
  bool progress = false;

//#############################################

//################ Read in args ###################

  std::vector<std::string> arguments(argv+1,argv+argc);

  if (arguments.empty())
    usage();

  std::vector<std::string> bagfile_names;

  for (auto arg = arguments.begin(); arg != arguments.end();arg++) 
  {
    if (*arg == "-h")
    {
      usage();
    }
    else if (*arg == "-c")
    {
      arg++;
      std::string compression_string = *arg;
      if (compression_string == "lz4")
        compression = rosbag::CompressionType::LZ4;
      else if (compression_string == "bz2")
        compression = rosbag::CompressionType::BZ2;
      else  if (compression_string == "none")
        compression = rosbag::CompressionType::Uncompressed;
      else
      {
        std::cerr << "[ERROR] invalid compression: " << *arg << std::endl;
        usage();
      }
    }
    else if (*arg == "-p")
    {
      progress = true;
    }

//################ Read in all non-args as paths to bags ###################

    else
    {
      std::ifstream test(*arg); 
      if (!test)
      {
        std::cerr << "[ERROR] File or path doesn't exist: " << *arg << std::endl;
        return -1;
      }
      else
      {
        bagfile_names.push_back(*arg);
      }
    }
  }

  for(auto in_name: bagfile_names)
  {
    if(in_name.size() <= 4 || in_name.substr(in_name.size()-4) != ".bag" )
    {
      std::cerr << "[ERROR] Not sure how to generate output file name from: " << in_name << std::endl;
      return -1;
    }

    if (progress)
      std::cout << "[INFO] Opening input bag " << in_name <<std::endl;

    rosbag::Bag inbag;
    inbag.open(in_name, rosbag::bagmode::Read);

    std::string out_name = in_name.substr(0, in_name.size()-4) + "_dedupe.bag";
    rosbag::Bag outbag;
    if (progress)
      std::cout << "[INFO] Opening output bag " << out_name <<std::endl;
    outbag.open(out_name, rosbag::bagmode::Write);

    if(progress)
    {
      std::cout << "[INFO] Setting compression to ";
      switch (compression)
      {
      case rosbag::compression::Uncompressed:
        std::cout << "none";
        break;
      case rosbag::compression::BZ2:
        std::cout << "BZ2";
        break;
      case rosbag::compression::LZ4:
        std::cout << "LZ4";
        break;
      default:
        std::cout << "unknown";
        break;
      }
      std::cout << std::endl;
    }

    outbag.setCompression(compression);

    rosbag::View view(true);
    view.addQuery(inbag);

    std::map<ros::StringPair, ros::Time> last_tf_written;

    progressbar bar(view.size());

    for(rosbag::MessageInstance const m: view)
    {
      if(progress)
        bar.update();
      if(m.getTopic() == "/tf")
      {
        try
        {
          auto tf_msg = m.instantiate<tf2_msgs::TFMessage>();
          bool dupe = false;
          if(tf_msg)
          {
            for (auto tf: tf_msg->transforms)
            {
              if(tf.header.stamp <= last_tf_written[std::make_pair(tf.header.frame_id, tf.child_frame_id)])
              {
                dupe = true;
                break;
              }
            }
            if(dupe)
              continue;
            for (auto tf: tf_msg->transforms)
              last_tf_written[std::make_pair(tf.header.frame_id, tf.child_frame_id)] = tf.header.stamp;
          }
          else
            std::cerr << "[Warning] Could not extract TFMessage from /tf topic" << std::endl;
        }
        catch (const std::exception& e)
        {
          std::cerr << "[Warning] Exception while extracting tf message: " << e.what() << std::endl;
        }
      }
      try
      {
        outbag.write(m.getTopic(), m.getTime(), m, m.getConnectionHeader());
      }
      catch (const std::exception& e)
      {
        std::cerr << "[Warning] Exception while writing: " << e.what() << std::endl;
      }
    }
    if (progress)
      std::cout << std::endl;
  }

  return 0;
}