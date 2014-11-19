from time import time, ctime

print 'Today is', ctime(time())
cmd = "ip addr show eth0 | grep inet | awk '{print $2}' | cut -d/ -f1 | head -n 1"

def	run_cmd(cmd):
	p = Popen(cmd, shell=True, stdout=PIPE)
	output = p.communicate()[0]
	return output

print run_cmd(cmd)
