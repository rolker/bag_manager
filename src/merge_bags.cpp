#include <rosbag/bag.h>
#include <rosbag/view.h>
#include "progressbar.hpp"


// -o, --output Output bag file
// -c, --compression Compression format: none, lw4 or bz2 (default lw4)
// -p, --progress Display progress
// -s, --start_time Skip messages earlier than start time
// -e, --end_time Skip messages later than end time
// -h, --help Display this message

std::string stampToString(const ros::Time& stamp, const std::string format="%H:%M")
{
  const int output_size = 100;
  char output[output_size];
  std::time_t raw_time = static_cast<time_t>(stamp.sec);
  struct tm* timeinfo = localtime(&raw_time);
  std::strftime(output, output_size, format.c_str(), timeinfo);
  return std::string(output);
}

int removeColon(std::string s)
{
  if (s.size() == 4) 
    s.replace(1, 1, "");
    
  if (s.size() == 5) 
    s.replace(2, 1, "");
    
  return stoi(s);
}

int main(int argc, char *argv[])
{
  //Defaults************************************
  std::string out_name = "out.bag";
  std::string compression = "lz4";
  bool progress = false;
  int start = 0000; 
  int end = 2400; 
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
      std::cout << "-s, --start_time Skip messages earlier than start time (HH:MM) 24hr" << std::endl;
      std::cout << "-e, --end_time Skip messages later than end time" << std::endl;
      std::cout << "-h, --help Display this message" << std::endl;
      return -1;
    }
    if (strcmp(*pargv,"-o") == 0)
    {
      pargv++;
      out_name = *pargv;
      arg_count = arg_count + 2;
    }
    if (strcmp(*pargv,"-c") == 0)
    {
      pargv++;
      compression = *pargv;
      arg_count = arg_count + 2;
    }
    if (strcmp(*pargv,"-p") == 0)
    {
     progress = true;
     arg_count++;
    }
    if (strcmp(*pargv,"-s") == 0)
    {
     pargv++;
     start = removeColon(*pargv);
     arg_count = arg_count + 2;
    }
    if (strcmp(*pargv,"-e") == 0)
    {
     pargv++;
     end = removeColon(*pargv);
     arg_count = arg_count + 2;
    }
  }
  rosbag::Bag outbag;
  outbag.open(out_name, rosbag::bagmode::Write);
  if(strcmp(const_cast<char*>(compression.c_str()),"none") == 0)
  {
  outbag.setCompression(rosbag::CompressionType::Uncompressed);
  }
  if(strcmp(const_cast<char*>(compression.c_str()),"bz2") == 0)
  {
  outbag.setCompression(rosbag::CompressionType::BZ2);
  }
  if(strcmp(const_cast<char*>(compression.c_str()),"lz4") == 0)
  {
  outbag.setCompression(rosbag::CompressionType::LZ4);
  }
  int size = 0;
  rosbag::View merged_view(true);
  std::vector<std::shared_ptr<rosbag::Bag> > bags;
  for(int i = arg_count; i < argc; i++)
  {
    if (progress == true)
    {
    std::cout << "[PROG] Opening bag " << argv[i] <<std::endl;
    }
    std::ifstream test(argv[i]); 
    if (!test)
    {
      std::cout << "[ERROR] File or path doesn't exist: " << argv[i] << std::endl;
      return -1;
    }
    bags.push_back(std::shared_ptr<rosbag::Bag>(new rosbag::Bag));
    bags.back()->open(argv[i], rosbag::bagmode::Read);
    merged_view.addQuery(*bags.back());
  }
  if(progress == true)
  {
    size = merged_view.size();
    std::vector<const rosbag::ConnectionInfo *> connection_infos = merged_view.getConnections();
    std::set<std::string> topics;
    BOOST_FOREACH(const rosbag::ConnectionInfo *info, connection_infos) {
    topics.insert(info->topic);
    }
    std::cout << "Merging topics: " << std::endl;
    for (auto it = topics.begin(); it != topics.end(); ++it)
    {
        std::cout << ' ' << *it << std::endl;
    }

  }
  progressbar bar(size);
  for(rosbag::MessageInstance const m: merged_view)
  {
    if(progress == true)
    {
      bar.update();
    }
    int time = removeColon(stampToString(m.getTime()));
    if( time > start && time < end )
    {
      outbag.write(m.getTopic(), m.getTime(), m, m.getConnectionHeader());
    }
  }
  std::cout << std::endl;
  return 0;
}
