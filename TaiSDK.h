

#define IN   
#define OUT


#define Tai_API(type) extern "C" _declspec(dllexport) type _stdcall





/*
��ʼ���� �����㷨��ʼ�������ݿ��ʼ��

����:
	dbname, ���ݿ�����

���:
	��

  ����: 
����0   �ɹ��� ���ض����������ĳ��ȣ��������ӿ��õ������ĵط��������СΪ�����������ĳ��ȵ�2����С
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
  */
Tai_API(int) face_init();



/*
�ر����ݿ⣬�����㷨ж��

  */
Tai_API(int) face_exit();


/*
��ͼ���ȡ��������

����:
	pic_sec64, ǰ�˿ؼ��ɼ���ͼ���base64��
	savePic,   ͼ�񱣴��ļ����� 0��ʾ������


���:
	feature, ���base64�����������������С����Ϊface_init����ֵ��2��

  ����: 
>=0   �ɹ�
<0  ʧ��
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
-50 �㷨��δ��ʼ��
  */

Tai_API(int) face_get_feature(char *pic_sec64,char *feature,char *savePic);


/*
��ͼ���ȡ��������

����:
	fname,     ͼ���ļ���

���:
	feature, ���base64�����������������С����Ϊface_init����ֵ��2��

  ����: 
>0   �ɹ�
<0  ʧ��
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
-50 �㷨��δ��ʼ��
  */
Tai_API(int) face_get_feature_from_image(char *fname,char *feature);



/*
���ڴ�ͼ���ȡ��������

����:
	pic_bin,     ͼ�������ڴ���
	pic_len��    ͼ�����ݳ���

���:
	feature, ���base64�����������������С����Ϊface_init����ֵ��2��

  ����: 
>0   �ɹ�
<0  ʧ��
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
-50 �㷨��δ��ʼ��
  */
Tai_API(int) face_get_feature_from_memory(char *pic_bin,int pic_len,char* feature);

/*
��ͼ���ȡ��������

����:
	pic_sec64, ǰ�˿ؼ��ɼ���ͼ���base64��
	savePic,   ͼ�񱣴��ļ����� 0��ʾ������


���:
	feature, ���base64�����������������С����Ϊface_init����ֵ��2��

  ����: 
>=0   �ɹ�
<0  ʧ��
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
-50 �㷨��δ��ʼ��
  */

Tai_API(int) face_get_feature(char *pic_sec64,char *feature,char *savePic);

/*
���������������жԱ�

����:
	feature1, ��������
	feature2, ��������


���:
	��

  ����: 
  �÷֣����127
>=0 �ɹ�
<0  ʧ��
-30 �����㷨��ʧ��
-40 ���ݿ��ʼ��ʧ��
-50 �㷨��δ��ʼ��
  */
Tai_API(int) face_comp_feature(char *feature1,char *feature2);

/*
���ͼ�����Ƿ��������

����:
	fname �ļ���

���:
	0 ����������
	1 ��������
*/
Tai_API(int) face_exist(char *fname);



/*
��ȡͼ�������������꣬ ���֧��15������

����:
	fname ͼ���ļ���

���:
	faceArr,����������ʱ�� ��������������

����:
	>0 ����������
	<=0 ���������������
*/
struct tagFaceCoord
{//�ܶ�����������������C1(x1, y1), C2(x2, y2)
    int x1;
    int y1;
    int x2;
    int y2;
};

Tai_API(int) face_get_pos(IN char *fname,OUT tagFaceCoord faceArr[15]);




/*
��ȡͼ��֡�����������꣬ ���֧��15������

����:
	frame_ptr ͼ��֡��ַ
	width,  ͼ����
	height��ͼ��߶�
	nCmo,   0=�ʣ�1=��
	nDpi,   ͼ��ֱ���

���:
	faceArr,����������ʱ�� ��������������

����:
	>0 ����������
	<=0 ���������������
*/

Tai_API(int) face_get_pos_from_frame(IN char *frame_ptr, IN int width, IN int height,IN int nCmo,int nDpi,OUT tagFaceCoord pFaceArr[15]);


Tai_API(int) face_comp_two_bin(char *pic_bin1,int pic_len1,char *pic_bin2,int pic_len2);
