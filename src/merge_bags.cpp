#include <rosbag/bag.h>
#include <rosbag/view.h>
#include "progressbar.hpp"
#include <ctime>
#include <iostream>


// -o, --output Output bag file
// -c, --compression Compression format: none, lw4 or bz2 (default lw4)
// -p, --progress Display progress
// -s, --start_time Skip messages earlier than start time
// -e, --end_time Skip messages later than end time
// -h, --help Display this message

int main(int argc, char *argv[])
{
  //Defaults************************************
  std::string out_name = "out.bag";
  rosbag::CompressionType compression = rosbag::CompressionType::LZ4 ;
  bool progress = false;
  ros::Time start = ros::TIME_MIN; 
  ros::Time end = ros::TIME_MAX;
  //********************************************

  int arg_count = 1;
  if (argc < 2) 
  {
    std::cout << "[ERROR]: No args, -h for help"<< std::endl;
    return -1;
  }
  for (char **pargv = argv+1; *pargv != argv[argc]; pargv++) 
  {
    if (strcmp(*pargv,"-h") == 0)
    {
      std::cout << "-o, --output Output bag file name" << std::endl;
      std::cout << "-c, --compression Compression format: none, lz4 or bz2 (default lz4)" << std::endl;
      std::cout << "-p, --progress Display progress" << std::endl;
      std::cout << "-s, --start_time Skip messages earlier than start time (Y-m-d-H:M:S) UTC" << std::endl;
      std::cout << "-e, --end_time Skip messages later than end time (Y-m-d-H:M:S) UTC" << std::endl;
      std::cout << "-h, --help Display this message" << std::endl;
      return -1;
    }
    if (std::string(*pargv) == "-o")
    {
      pargv++;
      out_name = *pargv;
      arg_count = arg_count + 2;
    }
    if (std::string(*pargv) == "-c")
    {
      pargv++;
      std::string compression_string = *pargv;
      if (compression_string == "lz4")
      {
        compression = rosbag::CompressionType::LZ4;
      }
      if (compression_string == "bz2")
      {
        compression = rosbag::CompressionType::BZ2;
      }
      if (compression_string == "none")
      {
        compression = rosbag::CompressionType::Uncompressed;
      }
      arg_count = arg_count + 2;
    }
    if (std::string(*pargv) == "-p")
    {
      progress = true;
      arg_count++;
    }
    if (std::string(*pargv) == "-s")
    {
      pargv++;
      arg_count = arg_count + 2;
      struct tm tm;
      strptime(*pargv, "%Y-%m-%d-%H:%M:%S",&tm);
      time_t start_time = mktime(&tm);
      start = ros::Time(start_time);
      if (progress == true)
      {
        std::cout << "[INFO] Merging with start time: " << start_time << std::endl;
      }
    }
    if (std::string(*pargv) == "-e")
    {
      pargv++;
      arg_count = arg_count + 2;
      struct tm tm;
      strptime(*pargv, "%Y-%m-%d-%H:%M:%S",&tm);
      time_t end_time = mktime(&tm);
      if (progress == true)
      {
        std::cout << "[INFO] Merging with start time: " << end_time << std::endl;
      }
    }
  }
  rosbag::Bag outbag;
  outbag.open(out_name, rosbag::bagmode::Write);
  outbag.setCompression(compression);
  int size = 0;
  rosbag::View merged_view(true);
  std::vector<std::shared_ptr<rosbag::Bag> > bags;
  for(int i = arg_count; i < argc; i++)
  { 
    if (progress == true)
    {
    std::cout << "[INFO] Opening bag " << argv[i] <<std::endl;
    }
    std::ifstream test(argv[i]); 
    if (!test)
    {
      std::cout << "[ERROR] File or path doesn't exist: " << argv[i] << std::endl;
      return -1;
    }
    bags.push_back(std::shared_ptr<rosbag::Bag>(new rosbag::Bag));
    bags.back()->open(argv[i], rosbag::bagmode::Read);
    merged_view.addQuery(*bags.back(),start,end);
    //double secs =ros::Time::now().toSec();
    //std::cout << secs << std::endl;
  }
  if(progress == true)
  {
    size = merged_view.size();
    std::vector<const rosbag::ConnectionInfo *> connection_infos = merged_view.getConnections();
    std::set<std::string> topics;
    for(const rosbag::ConnectionInfo* info: connection_infos) 
    {
    topics.insert(info->topic);
    }
    std::cout << "Merging topics: " << std::endl;
    for (auto it: topics)
    {
      std::cout << ' ' << it << std::endl;
    }

  }
  progressbar bar(size);
  for(rosbag::MessageInstance const m: merged_view)
  {
    if(progress == true)
    {
      bar.update();
    }
      outbag.write(m.getTopic(), m.getTime(), m, m.getConnectionHeader());
  }
  std::cout << std::endl;
  return 0;
}
