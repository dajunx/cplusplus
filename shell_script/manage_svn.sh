if [[ $1x == "start"x ]];then
	svnserve -d -r /opt/svn/test
elif [[ $1x == "stop"x ]];then
	kill -9 `ps -ef|grep svn | grep -v "$0"|grep -v "grep" | awk '{print $2}'`
elif [[ $1x == "ps"x ]];then
	ps -ef|grep svn | grep -v "$0"|grep -v "grep"
fi
