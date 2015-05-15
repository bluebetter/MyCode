#!/usr/bin/env python
#coding=utf-8
import sys
import urllib,urllib2
import re,os
import thread
import logging
import Queue
import time
import socket, struct

g_task_queue = Queue.Queue(0)
g_logger = None
THREAD_NUM = 10

AS_INFO = {
        'asn':None,
        'po_a':None,
        'po_4':None,
        'po_6':None,
        'pa_a':None,
        'pa_4':None,
        'pa_6':None,
        'bgppo_a':None,
        'bgppo_4':None,
        'bgppo_6':None,
        'ipso_4':None,
        'aspo_4':None,
        'aspo_6':None,
        'aaspl_a':None,
        'aaspl_4':None,
        'aaspl_6':None,
        'whois':None,
        'irr':None
        }

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

def get_cn_as_list():
    reg = '<td><a.+?</a></td>'
    reg_info = re.compile(reg)
    url = "http://bgp.he.net/country/CN"
    req = urllib2.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0 (X11; U; Linux i686)Gecko/20071127 Firefox/2.0.0.11')
    page = urllib2.urlopen(req)
    html = page.read()
    #print html
    for item in re.findall(reg_info, html):
        print item

def get_country():
    url = "http://bgp.he.net/AS4809"
    req = urllib2.Request(url)
    req.add_header('User-Agent', 'Mozilla/5.0 (X11; U; Linux i686)Gecko/20071127 Firefox/2.0.0.11')
    page = urllib2.urlopen(req)
    html = page.read()
    #print html
    #get_as_info(html)
    get_prefixes_v4(html)
    '''
    #Country of Origin
    reg = '<a href="/country.+?">.+?</a>'
    reg = '<a href="/country/CN">.+?</a>'
    reg_info = re.compile(reg)
    for item in re.findall(reg_info, html):
        print item
    '''

def get_prefixes_v4(html):
    s = 'nowrap'
    pos = html.find(s)
    while pos != -1:
        s2 = '<a href="/net/'
        head = html.find(s2, pos)
        head += len(s2)
        end = html.find('/', head)
        ip = html[head:end]
        head = end
        end = html.find('">', head)
        tmp_netmask = int(html[head+1:end])
        netmask = 0
        while tmp_netmask>0:
            netmask |= 1<<(32- tmp_netmask)
            tmp_netmask -= 1
        str_netmask = socket.inet_ntoa(struct.pack('I',socket.htonl(netmask)))
        head = html.find('<td>', end)
        head += 4
        end = html.find('<div', head)
        desc = html[head:end]
        
        head = html.find('title="', end)
        head += 7
        end = html.find('"', head)
        #print html[end:]
        country = html[head:end]

        desc = desc.strip('\t').strip('\n')

        print '%s\t%s\t%s\t%s'%(ip, str_netmask, desc, country)
        #pos = -1

        pos = html.find(s, pos+len(s))
    pass

def get_as_info(html):
    s = 'Prefixes Originated (all): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['po_a']= html[head:end].replace(',', '')
    s = 'Prefixes Originated (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['po_4']= html[head:end].replace(',', '')
    s = 'Prefixes Originated (v6): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['po_6']= html[head:end].replace(',', '')
    s = 'Prefixes Announced (all): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['pa_a']= html[head:end].replace(',', '')
    s = 'Prefixes Announced (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['pa_4']= html[head:end].replace(',', '')
    s = 'Prefixes Announced (v6): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['pa_6']= html[head:end].replace(',', '')
    s = 'BGP Peers Observed (all): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['bgppo_a']= html[head:end].replace(',', '')
    s = 'BGP Peers Observed (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['bgppo_4']= html[head:end].replace(',', '')
    s = 'BGP Peers Observed (v6): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['bgppo_6']= html[head:end].replace(',', '')
    s = 'IPs Originated (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['ipso_4']= html[head:end].replace(',', '')
    s = 'AS Paths Observed (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['aspo_4']= html[head:end].replace(',', '')
    s = 'AS Paths Observed (v6): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['aspo_6']= html[head:end].replace(',', '')
    s = 'Average AS Path Length (all): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['aaspl_a']= html[head:end].replace(',', '')
    s = 'Average AS Path Length (v4): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['aaspl_4']= html[head:end].replace(',', '')
    s = 'Average AS Path Length (v6): '
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('<br/>')
        end += head
        AS_INFO['aaspl_6']= html[head:end].replace(',', '')

    s = 'as-block'
    pos = html.find(s)
    if pos != -1:
        head = pos
        end = html[head:].find('</pre>')
        end += head
        AS_INFO['whois']= html[head:end].strip(' ')
    s = "irrsource'>"
    pos = html.find(s)
    if pos != -1:
        head = pos + len(s)
        end = html[head:].find('</span>')
        end += head
        tmp_irr = html[head:end].strip(' ')
        tmp_irr = tmp_irr.replace('</div>', '')
        tmp_irr = tmp_irr.replace('</a>', '')
        tmp_irr = tmp_irr.replace("<span class='irrdata'>", '')
        href_head = tmp_irr.find('<a href=')
        href_end = tmp_irr.find('>')
        tmp_irr = tmp_irr.replace(tmp_irr[href_head:href_end+1], '')
        tmp_irr = tmp_irr.replace('\t', '')
        AS_INFO['irr'] = tmp_irr

    for item in AS_INFO:
        print '%s:\t%s'%(item, AS_INFO[item])
        pass
    pass

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
    #get_cn_as_list()
    get_country()
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
