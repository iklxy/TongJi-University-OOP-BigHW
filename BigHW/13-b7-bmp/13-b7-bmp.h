/*2453359 李欣洋 信安*/
#pragma once

//不允许加入任何头文件，特别是<Windows.h>/<Wingdi.h>，查到就是0分甚至是倒扣-20 ！！！

//利用 pragma pack 确保结构体按字节对齐，防止编译器自动填充导致读取偏移错误
#pragma pack(push, 2) // 将当前字节对齐值压入栈，并设为2字节对齐（BMP文件头标准）

// 替代 BITMAPFILEHEADER
struct MyBitmapFileHeader {
    unsigned short bfType;      // 文件类型，必须是 0x4D42 ("BM")
    unsigned int   bfSize;      // 文件大小
    unsigned short bfReserved1; // 保留字1
    unsigned short bfReserved2; // 保留字2
    unsigned int   bfOffBits;   // 从文件头到实际位图数据的偏移字节数
};

// 替代 BITMAPINFOHEADER
struct MyBitmapInfoHeader {
    unsigned int   biSize;          // 本结构大小
    int            biWidth;         // 图像宽度
    int            biHeight;        // 图像高度
    unsigned short biPlanes;        // 平面数，总是1
    unsigned short biBitCount;      // 位深度 (1, 4, 8, 24, 32)
    unsigned int   biCompression;   // 压缩方式
    unsigned int   biSizeImage;     // 图像大小
    int            biXPelsPerMeter; // 水平分辨率
    int            biYPelsPerMeter; // 垂直分辨率
    unsigned int   biClrUsed;       // 使用的颜色数
    unsigned int   biClrImportant;  // 重要的颜色数
};

// 调色板颜色结构
struct MyRGBQuad {
    unsigned char rgbBlue;
    unsigned char rgbGreen;
    unsigned char rgbRed;
    unsigned char rgbReserved;
};

#pragma pack(pop)  // 恢复之前的对齐设置

// 简单的像素点结构，用于内部存储，方便绘制时统一处理
struct PixelPoint {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

class bitmap {
private:
    const bool is_debug;

    /* 私有成员数据 */
    int width;
    int height;
    PixelPoint* pixel_data; // 存储解析后的 RGB 数据
    bool is_valid;          // 标记是否成功加载

public:
    /* 公有部分不允许添加任何内容 */
    bitmap(const char* const filename, const bool is_debug);
    ~bitmap();
    int show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
        void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const;
};