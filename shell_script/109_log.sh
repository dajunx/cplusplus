#获取download下来的109组日志
cd 'C:\Users\Administrator\Desktop\109_log'
findTextReocordRes=$(find 'C:\Users\Administrator\Downloads' -name "*text_reocord*" | wc -l);
if [ $findTextReocordRes -eq 0 ]; then
    echo "[ERROR] 目录: C:\Users\Administrator\Downloads 没有找到任何东西，跳过[移动压缩包步骤]";
else
    findTextReocordRes=$(find 'C:\Users\Administrator\Downloads' -name "*text_reocord*");
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

    for file in "*$day*.tar.gz"; do 
        #由于主备切换原因，可能某一天的日志有两个ip开头的文件，故在for循环中分批加压同一天的日志
        for single_day in ${file[@]};do
            tar -zxf $single_day -C $day; 
        done
    done
    cd $day;

    #校验文件是否有24个，没有退出程序并提示错误
    everyDayFilesCount=$(ls | wc -l);
    if [ $everyDayFilesCount -ne 24 ]; then
        echo "[ERROR] 压缩包文件内容并没有包含完整一天的日志量，请检查... exit...";

        #测试用，退出程序前的清理工作
        cd ..;
        rm $day -rf;
        exit;
    fi

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
