#!/usr/bin/env python
#coding=utf-8
import sys
import urllib
import re,os
import thread
import logging
import Queue
import time

g_task_queue = Queue.Queue(0)
g_logger = None
THREAD_NUM = 10

def set_output(logpath, logname):
    global g_logger
    logger = logging.getLogger()
    if os.path.exists(logpath) == False:
        os.makedirs(logpath)
    hdlr = logging.FileHandler(logpath + '/' + logname)
    formatter = logging.Formatter('%(message)s')
    hdlr.setFormatter(formatter)
    logger.addHandler(hdlr)
    logger.setLevel(logging.NOTSET)
    g_logger = logger
    #g_logger.info("a")
    pass

def my_print(info):
    g_logger.info(info)
    pass

def get_isp(ip):
    reg = ('<li>本站主数据：(.+?)</li>').decode("utf-8").encode("gb2312")
    #reg = r'<li>(.+?)</li>'
    reg_info = re.compile(reg)

    url = "http://www.ip138.com/ips138.asp?ip=%s&action=2"%(ip)
    page = urllib.urlopen(url)
    html = page.read()
    #print html
    for item in re.findall(reg_info, html):
        print ("%s\t%s"%(ip, item.decode("gb2312").encode("utf-8")))

def t_get_isp(tid):
    while True:
        ip = g_task_queue.get()
        reg = ('<li>本站主数据：(.+?)</li>').decode("utf-8").encode("gb2312")
        #reg = r'<li>(.+?)</li>'
        reg_info = re.compile(reg)

        url = "http://www.ip138.com/ips138.asp?ip=%s&action=2"%(ip)
        page = urllib.urlopen(url)
        html = page.read()
        #print html
        for item in re.findall(reg_info, html):
            my_print("[%d]\t%s\t%s"%(tid, ip, item.decode("gb2312").encode("utf-8")))
    pass

def t_get_isp():
    i = 0
    while i<THREAD_NUM:
        thread.start_new_thread(get_isp, (i,))
        i += 1

def get_ip_list(filename):
    file = open(filename)
    for line in file.readlines():
        line = line.lstrip('\xef\xbb\xbf')
        line = line.rstrip('\n')
        line = line.rstrip('\r')
        g_task_queue.put(line)
        #get_isp(line)
    file.close()
    pass

if __name__ == "__main__":
    get_isp("58.249.125.217")
    '''
    set_output("/home/lxf/MyCode/python", "ip_info.rst")
    get_ip_list(sys.argv[1])

    #start thread
    t_get_isp()
    while True:
        print g_task_queue.qsize()
        time.sleep(1)
    '''
    pass
