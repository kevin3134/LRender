# Learning Notes
# 学习笔记

#### Matrix transfer and coordinates
object coordinates  
|| model  
world coordinates  
|| view  
eye coordinates  
|| projection  
clip coordinates (-1,1; still 3d coordinate, but we could assume front is the 2d image)  
|| viewport  
screen coordinates  

#### About GLM
View matrix, projection matrix used in this project are mainly from GLM.  
However, GLM matrix is different from matrix we commonly used, it is transposed.  
referrence: https://stackoverrun.com/cn/q/1237360  

#### 矩阵变化和坐标系
物体坐标系  
|| 模型矩阵  
世界坐标系  
|| 视图矩阵  
眼坐标系  
|| 投影矩阵  
剪切坐标系 (-1,1; still 3d coordinate, but we could assume front is the 2d image)  
|| 视角矩阵  
屏幕坐标系

#### 视图矩阵
视图矩阵控制观察的方向和距离（glLookat中相机的位置）  
对应函数为lrLookAt  

view matrix:

    right.x  right.y  right.z  -dot(right,eye)  
       up.x     up.y     up.z     -dot(up,eye)  
    front.x  front.y  front.z  -dot(front,eye)  
          0        0        0                1  


参考： http://www.songho.ca/opengl/gl_camera.html  


#### 投影矩阵

投影矩阵的远平面和近平面是相对于眼睛，在远平面外的物体是看不到的;
投影矩阵变换后，结果会在-1,1的一个立方体中
r = right; l=left; n = near; f = far
r-l 和 t-b 可以通过aspect(width/height)和fovy计算

参数：foxy, aspect(或者为right,left), near, far  
其中foxy为视角，影响看到物体的大小；aspect影响宽高比；near近平面，far远平面影响看到的范围

projection matrix:

    2/(r-l)        0         0  -(r+l)/(r-l)  
          0  2/(t-b)         0  -(t+b)/(t-b)  
          0        0  -2/(f-n)  -(f+n)/(f-n)  
          0        0         0             1 

 
参考： 
http://www.songho.ca/opengl/gl_projectionmatrix.html  
http://docs.gl/gl2/glOrtho  
http://learnwebgl.brown37.net/08_projections/projections_perspective.html  


#### 关于法线贴图  
对于rgb的直接法线贴图（tinyrender中做法似乎存在问题）
直接用读取后的法线方向和光照方向计算diff漫反射常数  
（rgb彩色贴图在旋转后就失效了，所以使用这类法线贴图时，对物体旋转会导致错误）  

对于切向空间法线贴图（可以避免选择的问题）
切向空间读取出的不是绝对法向量，是一种扰动，需要配合三角形的法线方向； 
计算时对读取的向量进行切向空间转世界坐标的矩阵转换。 

https://learnopengl-cn.github.io/05%20Advanced%20Lighting/04%20Normal%20Mapping/  


#### 关于高光贴图  
高光是一种材质类的贴图，要结合法线贴图一起使用

#### 关于齐次裁剪  
裁剪时机在投影矩阵后变成vec4(x,y,z,w)时，直接对比w和xyz判断是否需要裁剪；这样可以减少绘制次数，提高速度   
简单的方式是直接裁剪掉在区域外的三角形，但这会导致图形边缘不整齐  
完整的方法需要把裁剪的三角形变化为区域内（xyz都小于w的三角形） 

https://blog.csdn.net/softwarekid/article/details/45055253    
https://gameinstitute.qq.com/community/detail/117556    

#### 关于GLM
视图矩阵和投影矩阵来自GLM库，但是GLM库中的矩阵是和我们认知矩阵转置的  
参考： https://stackoverrun.com/cn/q/1237360  
