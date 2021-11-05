# Bag Manager
A set of tools designed for fast and efficient handling of ros bags

## merge_bags
This is a customizable bag merger tool using c++ allowing for the following arguments:\
`-o:  Output bag file name`\
`-c: Compression format: none, lz4 or bz2 (default lz4)`\
`-p: Display progress info`\
`-s: Merge messages after a certain time`\
`-e: Merge messages before a certain time`\
`-h: Display this message`
### Example: 
This example will create an lz4 compressed bagfile named out.bag, containing all of the data in the given directory that falls between 6:20 and 12:00 on 04/02/21\
`./merge_bags -p -c lz4 -s 2021-04-02-6:20:0 -e 2021-04-02-12:00:00 -o out.bag ~/project11/test_data/04_02_sample_data/*`

### Future Changes:
- Merge_bags filter by data type
- Filter by topic
