#获取download下来的109组日志
cd 'C:\Users\Administrator\Desktop\109_log'
findTextReocordRes=$(find 'C:\Users\Administrator\Downloads' -name "*text_reocord*")
if [ $findTextReocordRes ]; then
    mv $findTextReocordRes .;
    echo "move text reocord";
fi

#获取所有日志日期 格式 yyyy-mm-dd
logDays=$(ls -l | awk -F" " '{print $9}' | awk -F"reocord_" '{print $2}' | awk -F"." '{print $1}')

for day in ${logDays[@]}; do
    #把每天的日志压缩包解压到各自日期目录中
    mkdir $day;
    tar xf `echo "*$day*.tar.gz"` -C $day;
        #第二种实现方式
        #dailyLog=$(find . -name "*$day*.tar.gz");
        #tar xf ${dailyLog:2:58} -C $day;
    cd $day;

    #汇总文本为每天两个文件
    hourLogs=$(find . -name "*text_reocord*");
    cat `ls | grep "${hourLogs:2:45}0[0-9]"` > "${hourLogs:2:45}00~11";
    cat `ls | grep "${hourLogs:2:45}1[0-1]"` >> "${hourLogs:2:45}00~11";
    cat `ls | grep "${hourLogs:2:45}1[2-9]"` > "${hourLogs:2:45}12~23";
    cat `ls | grep "${hourLogs:2:45}2[0-3]"` >> "${hourLogs:2:45}12~23";
#rm $hourLogs; #删除源日志文件

    #校验
    #TODo

    #处理完毕 退出子目录 处理下一天日志 loop循环
    cd ..;
done
