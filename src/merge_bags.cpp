#include <rosbag/bag.h>
#include <rosbag/view.h>

int main(int argc, char *argv[])
{
  rosbag::Bag outbag;
  outbag.open("out.bag", rosbag::bagmode::Write);
  outbag.setCompression(rosbag::CompressionType::LZ4);

  rosbag::View merged_view(true);
  std::vector<std::shared_ptr<rosbag::Bag> > bags;
  for(int i = 1; i < argc; i++)
  {
    bags.push_back(std::shared_ptr<rosbag::Bag>(new rosbag::Bag));
    bags.back()->open(argv[i], rosbag::bagmode::Read);
    merged_view.addQuery(*bags.back());
  }

  for(rosbag::MessageInstance const m: merged_view)
  {
    outbag.write(m.getTopic(), m.getTime(), m, m.getConnectionHeader());
  }

  return 0;
}
