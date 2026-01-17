/*245359 李欣洋 信安*/
#include <iostream>
#include <iomanip>  //用于debug信息打印时的格式
#include <fstream>
//不再允许加入任何头文件，特别是<Windows.h>/<Wingdi.h>，查到就是0分甚至是倒扣-20 ！！！
using namespace std;

#include "13-b7-bmp.h"

/***************************************************************************
  函数名称：bitmap构造函数
  功    能：读取BMP文件并解析像素数据
  输入参数：文件名，调试标志
  返 回 值：无
  说    明：支持1, 4, 8, 24, 32位BMP
***************************************************************************/
bitmap::bitmap(const char* const filename, const bool is_debug) : is_debug(is_debug), pixel_data(NULL), width(0), height(0), is_valid(false)
{
    ifstream file(filename, ios::binary);
    if (!file.is_open())
    {
        is_valid = false;
        cout << filename << "打开失败" << endl;
        return;
    }

    MyBitmapFileHeader fileHeader;
    MyBitmapInfoHeader infoHeader;

    // 1. 读取文件头
    file.read((char*)&fileHeader, sizeof(fileHeader));
    if (fileHeader.bfType != 0x4D42) { // 'BM'
        if (is_debug) cerr << "打开失败" << endl;
        return;
    }

    // 2. 读取信息头
    file.read((char*)&infoHeader, sizeof(infoHeader));

    width = infoHeader.biWidth;
    height = infoHeader.biHeight; // BMP高度可能为负，表示从上到下存储，但一般为正（从下到上）
    int abs_height = (height > 0) ? height : -height;
    int bitCount = infoHeader.biBitCount;

    if (is_debug) {
        cout << "Width: " << width << ", Height: " << height << ", BitCount: " << bitCount << endl;
    }

    // 分配内部像素内存 (统一转为RGB存储)
    pixel_data = new PixelPoint[width * abs_height];

    // 3. 处理调色板 (对于 <= 8位的图像)
    MyRGBQuad* palette = NULL;
    if (bitCount <= 8) {
        int colorCount = 1 << bitCount;
        if (infoHeader.biClrUsed != 0) {
            colorCount = infoHeader.biClrUsed;
        }
        palette = new MyRGBQuad[colorCount];
        file.read((char*)palette, sizeof(MyRGBQuad) * colorCount);
    }

    // 4. 定位到像素数据区域
    file.seekg(fileHeader.bfOffBits, ios::beg);

    // 计算每行字节数 (必须是4的倍数)
    // 公式: ((width * bitCount + 31) / 32) * 4
    int lineByteSize = ((width * bitCount + 31) / 32) * 4;
    unsigned char* lineBuf = new unsigned char[lineByteSize];

    // 5. 读取并转换像素
    for (int i = 0; i < abs_height; ++i) {
        file.read((char*)lineBuf, lineByteSize);

        // 确定当前行在 pixel_data 中的索引
        int rowIdx = (height > 0) ? (abs_height - 1 - i) : i;
        PixelPoint* rowData = &pixel_data[rowIdx * width];

        int pixelIdx = 0;
        int byteIdx = 0;

        for (int w = 0; w < width; ++w) {
            unsigned char r = 0, g = 0, b = 0;

            if (bitCount == 1) {
                // 1位：1个字节存8个像素
                unsigned char byte = lineBuf[byteIdx];
                int bitOffset = 7 - (w % 8); // 高位在左
                int colorIndex = (byte >> bitOffset) & 0x1;
                if ((w + 1) % 8 == 0) byteIdx++; // 读完8个点，移动到下一字节

                r = palette[colorIndex].rgbRed;
                g = palette[colorIndex].rgbGreen;
                b = palette[colorIndex].rgbBlue;
            }
            else if (bitCount == 4) {
                // 4位：1个字节存2个像素
                unsigned char byte = lineBuf[byteIdx];
                int colorIndex;
                if (w % 2 == 0) { // 高4位
                    colorIndex = (byte >> 4) & 0x0F;
                }
                else { // 低4位
                    colorIndex = byte & 0x0F;
                    byteIdx++; // 读完2个点，移动到下一字节
                }
                r = palette[colorIndex].rgbRed;
                g = palette[colorIndex].rgbGreen;
                b = palette[colorIndex].rgbBlue;
            }
            else if (bitCount == 8) {
                // 8位：1个字节存1个像素
                int colorIndex = lineBuf[byteIdx++];
                r = palette[colorIndex].rgbRed;
                g = palette[colorIndex].rgbGreen;
                b = palette[colorIndex].rgbBlue;
            }
            else if (bitCount == 24) {
                // 24位：BGR
                b = lineBuf[byteIdx++];
                g = lineBuf[byteIdx++];
                r = lineBuf[byteIdx++];
            }
            else if (bitCount == 32) {
                b = lineBuf[byteIdx++];
                g = lineBuf[byteIdx++];
                r = lineBuf[byteIdx++];
                byteIdx++; 
            }

            rowData[w].r = r;
            rowData[w].g = g;
            rowData[w].b = b;
        }
    }

    if (palette) delete[] palette;
    delete[] lineBuf;

    // 修正 height 为正值，方便后续计算
    height = abs_height;
    is_valid = true;
    file.close();
}

/***************************************************************************
  函数名称：~bitmap
  功    能：析构函数，释放内存
  输入参数：
  返 回 值：
  说    明：
***************************************************************************/
bitmap::~bitmap()
{
    if (pixel_data) {
        delete[] pixel_data;
        pixel_data = NULL;
    }
}

/***************************************************************************
  函数名称：show
  功    能：在CMD窗口绘制图像
  输入参数：左上角x, 左上角y, 旋转角度, 是否镜像, 画点回调函数
  返 回 值：0表示成功
  说    明：处理坐标变换
***************************************************************************/
int bitmap::show(const int top_left_x, const int top_left_y, const int angle, const bool is_mirror,
    void (*draw_point)(const int x, const int y, const unsigned char red, const unsigned char green, const unsigned char blue)) const
{
    if (!is_valid || !pixel_data) return -1;

    // 遍历原始图像的每一个像素 (x_src, y_src)
    for (int y_src = 0; y_src < height; ++y_src) {
        for (int x_src = 0; x_src < width; ++x_src) {

            // 1. 获取像素颜色
            PixelPoint p = pixel_data[y_src * width + x_src];

            // 2. 处理镜像 (基于原始宽度)
            int x_curr = x_src;
            if (is_mirror) {
                x_curr = width - 1 - x_src;
            }

            // 3. 处理旋转 (逆时针)
            // 计算相对于 (0,0) 的新坐标
            int x_dest, y_dest;

            switch (angle) {
                case 90:
                    // 逆时针90度: (x, y) -> (y, W-1-x)
                    // 此时图像宽变高，高变宽
                    x_dest = y_src;
                    y_dest = width - 1 - x_curr;
                    break;
                case 180:
                    // 逆时针180度: (x, y) -> (W-1-x, H-1-y)
                    x_dest = width - 1 - x_curr;
                    y_dest = height - 1 - y_src;
                    break;
                case 270:
                    // 逆时针270度 (即顺时针90): (x, y) -> (H-1-y, x)
                    x_dest = height - 1 - y_src;
                    y_dest = x_curr;
                    break;
                case 0:
                default:
                    x_dest = x_curr;
                    y_dest = y_src;
                    break;
            }

            // 4. 加上屏幕偏移量
            int screen_x = top_left_x + x_dest;
            int screen_y = top_left_y + y_dest;

            // 5. 绘制
            draw_point(screen_x, screen_y, p.r, p.g, p.b);
        }
    }

    return 0;
}