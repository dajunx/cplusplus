if [[ $1x == "start"x ]];then
	svnserve -d -r /opt/svn/test
elif [[ $1x == "stop"x ]];then
	kill -9 `ps -ef|grep svn | grep -v "$0"|grep -v "grep" | awk '{print $2}'`
elif [[ $1x == "restart"x ]];then
	pid_name=$(ps -ef|grep svn | grep -v "$0"|grep -v "grep" | awk '{print $2}')
	if [[ $pid_name ]]; then
		kill -9 $pid_name; svnserve -d -r /opt/svn/test
	else
		svnserve -d -r /opt/svn/test
	fi
elif [[ $1x == "ps"x ]];then
	ps -ef|grep svn | grep -v "$0"|grep -v "grep"
fi
