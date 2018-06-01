#系统变量
targetDirectory='C:\Users\Administrator\Desktop\109_log'
FilesSourceDirectory='C:\Users\Administrator\Downloads'

#获取download下来的109组日志
MoveLogSourceFiles() {
    echo "[step1]: 移动压缩文件.";
    cd $targetDirectory;
    findTextReocordRes=$(find ${FilesSourceDirectory} -name "*text_reocord*" | wc -l);
    if [ $findTextReocordRes -eq 0 ]; then
        echo "[ERROR] 目录: C:\Users\Administrator\Downloads 没有找到任何东西，跳过[移动压缩包步骤]";
    else
        findTextReocordRes=$(find ${FilesSourceDirectory} -name "*text_reocord*");
        mv $findTextReocordRes .;
        echo "move text reocord";
    fi
}

#获取所有日志日期 格式 yyyy-mm-dd (末尾加入uniq目的是去除重复日期 毕竟有可能某天主备切换导致一天的日志放在不同服务器上面)
GetLogFileDates() {
    cd $targetDirectory;
    logDays=$(ls -l | awk -F" " '{print $9}' | awk -F"reocord_" '{print $2}' | awk -F"." '{print $1}' | sort | uniq)
}

DecompressLogFiles() {
    for day in ${logDays[@]}; do
        #把每天的日志压缩包解压到各自日期目录中
        mkdir $day;
    
        for file in "*$day*.tar.gz"; do 
            #由于主备切换原因，可能某一天的日志有两个ip开头的文件，故在for循环中分批加压同一天的日志
            for single_day in ${file[@]};do
                tar -zxf $single_day -C $day; 
            done
        done
    done
}

#每日目录下需要的动作：
    #1)check 
    #2)整理日志
DailyAction() {
    cd $targetDirectory;
    destDirectory="109_log"; #汇总目录
    mkdir $destDirectory;

    GetLogFileDates
    for day in ${logDays[@]}; do
        cd $day;
        checkAction $day #需要check返回值
        if [ $? -ne 1 ]; then
            gatherDailyLogs $day
        else 
            exit;
        fi
        cd ..;
    done
}

#校验某天文件夹目录下，文件是否有24个，没有退出程序并提示错误
checkAction() {
    everyDayFilesCount=$(ls | wc -l);
    day=$1
    if [ $everyDayFilesCount -ne 24 ]; then
        echo "[ERROR] $day 时间的压缩包文件内容并没有包含完整一天的日志量，请检查... exit...";
        echo "everyDayFilesCount count:"$everyDayFilesCount;
        return 1;
    fi
    return 0;
}

gatherDailyLogs() {
    #汇总文本为每天两个文件 (sort目的: 按照小时排序grep出来的文件 便于汇总，使最终文件内容是按照时间排序)
    day=$1
    cat `ls | grep ".*text_reocord.log.${day}-0[0-9].*\|.*text_reocord.log.${day}-1[0-1].*" | sort -n -k 4 -t -` > "text_reocords.log.${day}-00~11"; #TODO 若找不到需要grep的文件，shell会阻塞掉
    cat `ls | grep ".*text_reocord.log.${day}-1[2-9].*\|.*text_reocord.log.${day}-2[0-3].*" | sort -n -k 4 -t -` > "text_reocords.log.${day}-12~23";

    #拷贝日志到汇总目录
    cp text_reocords.log* ../109_log -rf;
}

#日志处理需求并非都是完整的，有可能只处理做一部分功能
case "$1" in
    'all' )
        MoveLogSourceFiles
        DecompressLogFiles
        DailyAction
        ;;
    'move' )
        MoveLogSourceFiles
        ;;
    'gatherlogs' )
        DailyAction
        ;;
    *)
        echo "Usage: $0 {all|move|gatherlogs}" >&2
        ;;
esac
