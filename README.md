# HW2
for special project. 

##Introduction 
  因為這次在用Visual Studio的時候遇到很多問題，像是標頭檔之間的連結都抓不到，還有編譯時不斷要求不知道在哪的"stdafx.h"等等問題，，只好  用HW1的專案來開，並且把所有的濾波器用不同函示分開寫才解決。所有的結果都在/SP_HW2/TestImg裡。


##Specification
1. Linear filter : 以線性處理原始圖像。
2. Nonlinear filter : 以非線性處理原始圖像。
3. Sobel  filter : 一階微分，並且補強中央數值，凸顯不連續的地方。


##Concept Development
    -所有的filter都要是causal的才行，所以當作用在最邊緣的pixel上時，會發生padding的情況，這樣就會需要預測未來的訊號，不過我認為邊緣其實肉眼辨識不出來，所以只要不在四邊上作用就好了，直接將值設定成0即可。
    -若需要用到矩陣，都設定成對稱矩陣，在摺積時可以簡化翻轉的步驟。
1. Linear filter :  對稱。
2. Nonlinear filter : 取中心點3*3範圍內最大、最小或中間值。
3. Sobel  filter : 把Gx和Gy相加。


##Implementation
1. Linear filter  
    試了兩種不同的filter，一次是[111  111  111]，將九個值加起來後要再除以九，可避免overflow。
![linearOutput](/SP_HW2/TestImg/linearOutput.png)  
    另一次是[0-10  -15-1  0-10]，中心點一定要有正的值才不會變太多，其他一定要有負的值才可以平衡亮度，不然會變得很亮；如果每一項都成(-1)
    ，並且把中心的值調低一點會有負片的效果，按次序表現如下圖。  
![ShrapenOutput.png](/SP_HW2/TestImg/SharpenOutput.png)  
![linearinverseOutput.png](/SP_HW2/TestImg/linearinverseOutput.png)  
2. Nonlinear filter
    取最大值  
![nonlinearmaxOutput.png](/SP_HW2/TestImg/nonlinearmaxOutput.png)   
    取最小值  
![nonlinearminOutput.png](/SP_HW2/TestImg/nonlinearminOutput.png)   
    取中間值  
![nonlinearmedOutput.png](/SP_HW2/TestImg/nonlinearmedOutput.png)   
3. Sobel filter
    分為X-方向加強邊界(Gx)和為Y-方向加強邊界(Gy)，理論上的G = (Gx^2 + Gy^2)^0.5，不過為了簡化運算，可以改成G = |Gx| + |Gy|。  
    先對x方向做sobel。   
![SobelxOutput.png](/SP_HW2/TestImg/SobelxOutput.png)     
    再對y方向做sobel。   
![SobelyOutput.png](/SP_HW2/TestImg/SobelyOutput.png)     
直接相加。   
![SobelOutput.png](/SP_HW2/TestImg/SobelOutput.png)     
    灰階處理後再sobel，比較不雜亂。。  
![SobelgrayOutput.png](/SP_HW2/TestImg/SobelgrayOutput.png)     

##Discussion
* 由於原圖片的解析度大概400多，可以不需要考慮太大的filter，才不會造成影像失真。
* linear filter中[111  111  111]因為是和附近平均的結果，看起來就像是像素較低的影像。
    而[0-10  -15-1  0-10]是把中心點突出，和上下左右的pixel拉大差距，對比變強，讓色彩看起來更銳利。
* Nonlinear filter中也是讓附近的值都相同，所以看起來也很粗糙，取最大值會讓畫面看起來比較亮，最小值最暗，取中間值的亮度和色彩和原版最相似。
* sobel filter中，先經過灰階處理會比較清楚，才不會有其他色彩干擾；因為G = |Gx| + |Gy|和G = (Gx^2 + Gy^2)^0.5兩種運算方法還是有差，例如一些斜向的邊界會被化簡，不過，整體上看起來還是蠻正確的。   


##Reference
1. http://blog.yam.com/chu24688/article/50729404
2. http://docs.opencv.org/2.4.2/doc/tutorials/core/mat-mask-operations/mat-mask-operations.html#the-filter2d-function
