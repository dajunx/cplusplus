#获取download下来的109组日志
cd 'C:\Users\Administrator\Desktop\109_log'
cp `find 'C:\Users\Administrator\Downloads' -name "xx.xx.xx.xx*"` .

#获取所有日志日期 格式 yyyy-mm-dd
logDays=$(ls -l | awk -F" " '{print $9}' | awk -F"reocord_" '{print $2}' | awk -F"." '{print $1}')

#创建目录并拷贝对应日志压缩包进去
for day in ${logDays[@]}; do
    #生成日期目录并拷贝对应压缩包、解压
    mkdir $day;
    dailyLog=$(find . -name "*$day*.tar.gz");
    cp $dailyLog $day;
    cd $day;
    tar xf $dailyLog;
    rm $dailyLog;

    #汇总文本为每天两个文件 hourLogs每日原始日志文件集合
    hourLogs=$(find . -name "xx.xx.xx.xx*");
    cat `ls | grep "${hourLogs:2:45}0[0-9]"` > "${hourLogs:2:45}00~11";
    cat `ls | grep "${hourLogs:2:45}1[0-1]"` >> "${hourLogs:2:45}00~11";
    cat `ls | grep "${hourLogs:2:45}1[2-9]"` > "${hourLogs:2:45}12~23";
    cat `ls | grep "${hourLogs:2:45}2[0-3]"` >> "${hourLogs:2:45}12~23";
    rm $hourLogs; #删除源日志文件

    #校验
    #TODo

    #处理完毕 退出子目录 处理下一天日志 loop循环
    cd ..;
done
