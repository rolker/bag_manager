# BAG PRO

1. [ bag_merge ](#bag_merge)
2. [ msgext ](#msgext)

## About
This is a set of specialized tools for ROS .bag post processing. These tools are built specifically for the uses of the lab that I work in, with our vehicles, they are mostly likely not optimized or well documented. With that said, if you find them useful, want to contribute, or have suggestions, I'll do my best to maintain the repo.

## Installation
Installation of these utilities may vary, some of them are built as ros packages, intended to be imported and installed into an existing ros workspace. Some of them can be built/run independently. Vague instructions will be included in script details bellow. 

## Scripts

This is a running list of the scripts, along with some info of what they're for, some installation instructions, etc.


<a name="#bag_merge"></a>

# bag_merge

Bag_merge was created to simply combine multiple bag files into one at a reasonable speed. This is because in many applications bag files are given time or size caps and are split into multiple bag files. When analyzing bag files after an operation/deployment/test it is often very useful to be able to see data that spans across multiple bag files. 
This has been done in python before, however when merging large bags it can be quite slow in python.

##### Installation
Bag_merge is structured to be a ROS package as it was originally intended to be run on vehicle to streamline ROS bag export, its only been testing with noetic on Ubuntu 20.04. 
If you're merging bag files, chances are you have a ROS worksapce setup but in the chance you don't heres a guide: http://wiki.ros.org/noetic/Installation
1. `git clone https://github.com/munzz11/bag_pro.git` into an existing ROS workspace
2. Compile ROS workspace
3. Add to path if you want to call from elsewhere. Example: (`export PATH=$PATH:~/project11/catkin_ws/devel/lib/bag_manager/`)
4. Run with `merge_bags` or `./merge_bags`

##### Arguments 
`-o, --output Output bag file name`<br />
`-c, --compression Compression format: none, lz4 or bz2 (default lz4)`<br />
`-p, --progress Display progress`<br />
`-s, --start_time Skip messages earlier than start time (Y-m-d-H:M:S) (local time)`<br />
`-e, --end_time Skip messages later than end time (Y-m-d-H:M:S) (local time`<br />
`-x, --exclude Exclude a topic, may be repeated`<br />
`-i, --include Ixclude a topic, may be repeated`<br />
`-t, --topic Topic filtering config file`<br />
`-h, --help Display this message`<br />

You could probably compile and run this outside of a ROS workspace with the necessary rosdeps  `¯\_(ツ)_/¯`

##### Demo
![Alt Text](https://i.ibb.co/tYWq8qv/merge-bag-example.gif)

<a name="#msgext"></a>

# msgext
This tool was created to extract the the .msg definitions from a ROS .bag file. Custom message definitions are somewhat common in ROS, but if you are doing post processing or working on a subsystem of someone elses roscore, you may not always have the custom definitions to compile in your workspace, this tools intends to automate extracting message definitions from a rosbag building a rudimentary workspace with all the message definitions from the bag file.

##### Installation 
This script is *not* a ros package. Install via:
1. `git clone https://github.com/munzz11/bag_pro.git` 
2. `cd bag_pro`
3. `sudo python3 setup.py install`

##### Arguments
`usage: msgext [-h] [-v] [-w] [-n N] bag [bag ...]`<br />

&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`Read, analyze and build temp workspaces containing .msg definitions from a ROS .bag file`<br />

`positional arguments:`<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`bag         Path of a bag file or a folder of bag files.`<br />

`optional arguments:`<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`-h, --help  show this help message and exit`<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`-v          Verbose mode, prints msg definitions as they're read (default: False)`<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`-w          Workspace mode, automatically generates a ROS workspace containing msg definitions (default: False)`<br />
&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;`-n N        Name of workspace folder, default is 'workspace' (default: None)`<br />

##### Demo
![Alt Text](https://i.ibb.co/gySYXmV/msgext-example.gif)
