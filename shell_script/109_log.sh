#获取download下来的109组日志
cd 'C:\Users\Administrator\Desktop\109_log'
#如下有个问题，当find 未查到文件时，${#findTextReocordRes[@]} 非 0而为1，费解
#TODO
findTextReocordRes=$(find 'C:\Users\Administrator\Downloads' -name "*text_reocord*")
if [ ${#findTextReocordRes[@]} -ne 0 ]; then
    mv $findTextReocordRes .;
    echo "move text reocord";
fi

#获取所有日志日期 格式 yyyy-mm-dd (末尾加入uniq目的是去除重复日期 毕竟有可能某天主备切换导致一天的日志放在不同服务器上面)
logDays=$(ls -l | awk -F" " '{print $9}' | awk -F"reocord_" '{print $2}' | awk -F"." '{print $1}' | uniq)

destDirectory="109_log";
mkdir $destDirectory;

for day in ${logDays[@]}; do
    #把每天的日志压缩包解压到各自日期目录中
    mkdir $day;
#tar xf `echo "*$day*.tar.gz"` -C $day;
    for file in *.tar.gz; do tar -zxf $file -C $day; done
        #第二种实现方式
        #dailyLog=$(find . -name "*$day*.tar.gz");
        #tar xf ${dailyLog:2:58} -C $day;
    cd $day;

    #汇总文本为每天两个文件 (sort目的: 按照小时排序grep出来的文件 便于汇总，使最终文件内容是按照时间排序)
#hourLogs=$(find . -name "*text_reocord*");
    cat `ls | grep ".*text_reocord.log.${day}-0[0-9].*\|.*text_reocord.log.${day}-1[0-1].*" | sort -n -k 4 -t -` > "text_reocords.log.${day}-00~11";
    cat `ls | grep ".*text_reocord.log.${day}-1[2-9].*\|.*text_reocord.log.${day}-2[0-3].*" | sort -n -k 4 -t -` > "text_reocords.log.${day}-12~23";

#rm $hourLogs; #删除源日志文件

    #校验
    #TODo

    #处理完毕 退出子目录 处理下一天日志 loop循环
    cd ..;

    #拷贝日志到汇总目录
    cp ./${day}/text_reocords.log* $destDirectory -rf;
done

#打包日志文件内容
#tar czvf ${destDirectory}.tar.gz $destDirectory
