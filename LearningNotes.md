# Learning Notes
# 学习笔记

## Matrix transfer and coordinates
object coordinates  
|| model  
world coordinates  
|| view  
eye coordinates  
|| projection  
clip coordinates (-1,1; still 3d coordinate, but we could assume front is the 2d image)  
|| viewport  
screen coordinates  

## About GLM
View matrix, projection matrix used in this project are mainly from GLM.  
However, GLM matrix is different from matrix we commonly used, it is transposed.  
refer about this: https://stackoverrun.com/cn/q/1237360  

## 矩阵变化和坐标系
物体坐标系  
|| 模型矩阵  
世界坐标系
|| 视图矩阵 
眼坐标系
|| 投影矩阵  
剪切坐标系 (-1,1; still 3d coordinate, but we could assume front is the 2d image)  
|| 视角矩阵  
屏幕坐标系

## 投影矩阵
投影矩阵的远平面和近平面是相对于眼睛，在远平面外的物体是看不到的;
投影矩阵变换后，结果会在-1,1的一个立方体中
参考： http://www.songho.ca/opengl/gl_projectionmatrix.html

## 关于GLM
视图矩阵和投影矩阵来自GLM库，但是GLM库中的矩阵是和我们认知矩阵转置的  
参考： https://stackoverrun.com/cn/q/1237360  
