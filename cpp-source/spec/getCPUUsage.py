    
# 不准
# import wmi
# def read_cpu_usage():
#     c = wmi.WMI()
#     for cpu in c.Win32_Processor():
#         return cpu.LoadPercentage
    
# print ("111111111111111111")
# print( read_cpu_usage())
# # input('按任意键继续')

# 不准
# import psutil 
# import time
# # cpu_res = psutil.cpu_percent()
# # print(cpu_res)
# # 每一秒获取获取cpu的占有率 --->持久化保存
# # 如何将时间和对应的cpu占有率去匹配
# while True:
#     # 获取当前时间和cpu的占有率
#     t = time.localtime()
#     cpu_time = '%d:%d:%d' % (t.tm_hour, t.tm_min, t.tm_sec)
#     cpu_res = psutil.cpu_percent()
#     print(cpu_res)
#     # 保存在文件中
#     # with open('cpu.txt', 'a+') as f:
#     #     f.write('%s %s \n' % (cpu_time, cpu_res))
#     time.sleep(1)、




# import os
# def get_info(metric):
#     metric_cmd_map = {
#         "cpu_usage_rate": "wmic cpu get loadpercentage",
#         "mem_total": "wmic ComputerSystem get TotalPhysicalMemory",
#         "mem_free": "wmic OS get FreePhysicalMemory"
#     }
#     out = os.popen("{}".format(metric_cmd_map.get(metric)))
#     value = out.read().split("\n")[2]
#     out.close()
#     return float(value)

# # cpu使用率
# cpu_usage_rate = get_info('cpu_usage_rate')
# print("windows的CPU使用率是{}%".format(cpu_usage_rate))


import psutil
import time


def getMemory():
    data = psutil.virtual_memory()
    memory = str(int(round(data.percent))) + "%"
    return memory

def getCpu():
    cpu = str(round(psutil.cpu_percent(interval=1), 2)) + "%"
    return cpu


def main():
    while (True):
        memory = getMemory()
        cpu = getCpu()
        time.sleep(0.2)
        print("CPU占用：%s 	 内存占用：%s"%(cpu, memory))


if __name__ == "__main__":
    main()