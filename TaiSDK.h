

#define IN   
#define OUT


#define Tai_API(type) extern "C" _declspec(dllexport) type _stdcall





/*
初始化， 包括算法初始化和数据库初始化

输入:
	dbname, 数据库名称

输出:
	无

  返回: 
大于0   成功， 返回二进制特征的长度，在其他接口用到特征的地方，建议大小为二进制特征的长度的2倍大小
-30 加载算法库失败
-40 数据库初始化失败
  */
Tai_API(int) face_init();



/*
关闭数据库，人脸算法卸载

  */
Tai_API(int) face_exit();


/*
从图像获取人脸特征

输入:
	pic_sec64, 前端控件采集的图像的base64串
	savePic,   图像保存文件名， 0表示不保存


输出:
	feature, 输出base64编码的人脸特征，大小建议为face_init返回值的2倍

  返回: 
>=0   成功
<0  失败
-30 加载算法库失败
-40 数据库初始化失败
-50 算法尚未初始化
  */

Tai_API(int) face_get_feature(char *pic_sec64,char *feature,char *savePic);


/*
从图像获取人脸特征

输入:
	fname,     图像文件名

输出:
	feature, 输出base64编码的人脸特征，大小建议为face_init返回值的2倍

  返回: 
>0   成功
<0  失败
-30 加载算法库失败
-40 数据库初始化失败
-50 算法尚未初始化
  */
Tai_API(int) face_get_feature_from_image(char *fname,char *feature);



/*
从内存图像获取人脸特征

输入:
	pic_bin,     图像数据内存区
	pic_len，    图像数据长度

输出:
	feature, 输出base64编码的人脸特征，大小建议为face_init返回值的2倍

  返回: 
>0   成功
<0  失败
-30 加载算法库失败
-40 数据库初始化失败
-50 算法尚未初始化
  */
Tai_API(int) face_get_feature_from_memory(char *pic_bin,int pic_len,char* feature);

/*
从图像获取人脸特征

输入:
	pic_sec64, 前端控件采集的图像的base64串
	savePic,   图像保存文件名， 0表示不保存


输出:
	feature, 输出base64编码的人脸特征，大小建议为face_init返回值的2倍

  返回: 
>=0   成功
<0  失败
-30 加载算法库失败
-40 数据库初始化失败
-50 算法尚未初始化
  */

Tai_API(int) face_get_feature(char *pic_sec64,char *feature,char *savePic);

/*
两个人脸特征进行对比

输入:
	feature1, 人脸特征
	feature2, 人脸特征


输出:
	无

  返回: 
  得分，最大127
>=0 成功
<0  失败
-30 加载算法库失败
-40 数据库初始化失败
-50 算法尚未初始化
  */
Tai_API(int) face_comp_feature(char *feature1,char *feature2);

/*
检查图像中是否存在人脸

输入:
	fname 文件名

输出:
	0 不存在人脸
	1 存在人脸
*/
Tai_API(int) face_exist(char *fname);



/*
获取图像中人脸的坐标， 最多支持15个人脸

输入:
	fname 图像文件名

输出:
	faceArr,在有人脸的时候， 返回人脸的坐标

返回:
	>0 人脸的数量
	<=0 不存在人脸或出错
*/
struct tagFaceCoord
{//能定人脸面积区域的两点C1(x1, y1), C2(x2, y2)
    int x1;
    int y1;
    int x2;
    int y2;
};

Tai_API(int) face_get_pos(IN char *fname,OUT tagFaceCoord faceArr[15]);




/*
获取图像帧中人脸的坐标， 最多支持15个人脸

输入:
	frame_ptr 图像帧地址
	width,  图像宽度
	height，图像高度
	nCmo,   0=彩，1=灰
	nDpi,   图像分辨率

输出:
	faceArr,在有人脸的时候， 返回人脸的坐标

返回:
	>0 人脸的数量
	<=0 不存在人脸或出错
*/

Tai_API(int) face_get_pos_from_frame(IN char *frame_ptr, IN int width, IN int height,IN int nCmo,int nDpi,OUT tagFaceCoord pFaceArr[15]);


Tai_API(int) face_comp_two_bin(char *pic_bin1,int pic_len1,char *pic_bin2,int pic_len2);
