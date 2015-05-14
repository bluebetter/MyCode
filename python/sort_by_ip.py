#!/usr/bin/env python
#coding=utf-8
import sys

def sort_by_ip(ip_file, info_file):
    INFO_DICT = { }
    f = open(info_file)
    for line in f.readlines():
        line = line.lstrip('\xef\xbb\xbf')
        line = line.rstrip('\n')
        line = line.rstrip('\r')
        s = line.split('\t')
        INFO_DICT[s[1]] = ''.join(s[2:])
        #print '\t'.join(s[1:])
    f.close()

    file = open(ip_file)
    for line in file.readlines():
        line = line.lstrip('\xef\xbb\xbf')
        line = line.rstrip('\n')
        line = line.rstrip('\r')
        if line in INFO_DICT:
            print "%s\t%s"%(line, INFO_DICT[line])
    file.close()

if __name__ == '__main__':
    #sort_by_ip(sys.argv[1], sys.argv[2])
    sort_by_ip("ip_list.txt", "ip_info.rst")
    pass
