#pragma once
//自定义结构体,如颜色、二维向量、三维向量、游戏对象、三维平面、矩阵、多边形、渲染列表、透视体等
namespace T3D
{
	struct TColor;					//颜色
	struct TVector2D;				//二维向量
	struct TVector3D;				//三维向量
	struct TVector4D;				//四维向量
	struct TMatrix44;				//4x4矩阵

	struct TVertex4D;					//顶点
	struct TPolygon4D;				//基于顶点多边形
	struct TPolygonF4D;				//基于索引多边形
	struct TObject4D;				//对象
	struct TPlane;					//三维平面
	struct TRenderList4D;			//渲染列表
	
	struct TFrustum;				//透视体
}