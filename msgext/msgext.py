#!/usr/bin/env python3

import rosbag
import sys
import argparse
import os
import glob

def mkdir(string):
    direct = base_path
    direct_arr = string.split('/')

    for i in direct_arr:
        direct = (direct + '/' + i)
        if i in direct_arr[:-1]:
            try:
                os.mkdir(direct)
                print('Created: ' + direct)
            except FileExistsError:
                print(' ')
    return(direct)


def main():
    global base_path
    base_path = 'workspace' #base folder location, defaults to 'workspace' in working directory
    types = {}

    parser = argparse.ArgumentParser(description='Read, analyze and build temp workspaces containing .msg definitions from a ROS .bag file,.', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
    parser.add_argument('bag', 
                        nargs='+', 
                        help='Path of a bag file or a folder of bag files.')
    parser.add_argument("-v", help="Verbose mode, prints msg definitions as they're read", 
                        action="store_true")
    parser.add_argument("-w", help="Workspace mode, automatically generates a ROS workspace containing msg definitions", 
                        action="store_true")
    parser.add_argument("-n", help="Name of workspace folder, default is 'workspace'")
    args = parser.parse_args()

    full_paths = [os.path.join(os.getcwd(), path) for path in args.bag]
    files = set()
    for path in full_paths:
        if os.path.isfile(path):
            files.add(path)
            print('[INFO] Opening bag: ' + path + '\n')
        else:
            files |= set(glob.glob(path + '/*' + args.extension))

    # print(args.bag)
    
    if args.n:
        base_path = args.n 

    for bagname in files:
        
        bag = rosbag.Bag(bagname)
        for topic, msg, t in bag.read_messages():
            types[msg._type] = msg._full_text 

    if args.w:
        try:
            os.mkdir(base_path)
        except FileExistsError:
            print()

    for t in types:

        # Print msg def to std out #
        if args.v:
            print ("Message type:", t)
            print ("Message text:")
            print (types[t])
            print ()
        ############################
        if args.w:
            direct = mkdir(t)
            f = open(direct + '.msg', 'w+')
            lines = types[t].split('MSG: ',-1)
            first = True
            for x in lines:
                if first:
                    f.write(x)
                    first = False
                    f.close()
                else:
                    sublines = x.split('\n',-1)
                    path = sublines[0].strip('MSG: ')
                    
                    subtype = mkdir(path)
                    f = open(subtype + '.msg', 'w+')
                    first2 = True
                    for i in sublines:
                        if(first2):
                            first2 = False
                        else:
                            f.write(i + '\n')
                    f.close()
      
if __name__ == '__main__':
    main()      


    
