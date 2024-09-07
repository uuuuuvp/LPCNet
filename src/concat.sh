# 遍历当前目录下的所有.wav文件  
for i in ./*.wav; do  
    # 获取文件名和扩展名  
    filename=$(basename -- "$i")  
    extension="${filename##*.}"  
    filename="${filename%.*}"  
      
    # 构造输出文件名  
    output_file="$filename.s16"  
      
    # 使用ffmpeg将.wav文件转换为8000 Hz, 单声道, 16位PCM格式  
    # 注意：ffmpeg通常不会直接输出为.s16扩展名，但我们可以将其视为16位PCM格式的音频  
    ffmpeg -i "$i" -ar 8000 -ac 1 -f s16le "$output_file"  
done  
