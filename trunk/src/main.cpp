#include <stdio.h>
#include <random>
#include <iostream>
#include <afx.h>
#include <fstream>

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include "ROCCAT_Talk.h"
#pragma comment(lib, "C:/Programming/lib/Roccat Talk SDK.lib")
int main(int argc, char** argv)
{
	std::cout<<"*    ___  ____ _  _ _  _ "<<std::endl;
	std::cout<<"*    |__] |    |\\ |  \\/  "<<std::endl;
	std::cout<<"*    |    |___ | \\| _/\\_  "<<std::endl;
	std::cout<<"-----------------------------"<<std::endl;	
	std::cout<<"leetlights v0.1"<<std::endl;

	CROCCAT_Talk roccat;
	roccat.RestoreLEDRGB();
	const int bins = 15;
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int miniScreenY = nScreenHeight/10;
	const int miniScreenX = nScreenWidth/10;
	
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, miniScreenX, miniScreenY);
	SelectObject(hCaptureDC,hCaptureBitmap); 
	SetStretchBltMode(hCaptureDC,HALFTONE);
	int histogram[bins+1][bins+1][bins+1];
		COLORREF* pixel = new COLORREF [ miniScreenX * miniScreenY ];
	
		int lR=0;
		int lG=0;
		int lB=0;
	DWORD start = GetTickCount();
	DWORD diff;
	DWORD lastTime = start+1;
	DWORD time;
	while(true)
	{
		time = GetTickCount();
		diff= start-time ;
		lastTime = time ;
		if(!StretchBlt(hCaptureDC,0,0,miniScreenX,miniScreenY,hDesktopDC,0,0,nScreenWidth,nScreenHeight,SRCCOPY))
		{
			std::cout<<"Capturing failed, skipping iteration"<<std::endl;
			Sleep(100);
			continue;
		}

		BITMAPINFO MyBMInfo = {0};


		BITMAPINFO bmpInfo;
		bmpInfo.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
		bmpInfo.bmiHeader.biWidth = miniScreenX;
		bmpInfo.bmiHeader.biHeight = miniScreenY;
		bmpInfo.bmiHeader.biPlanes = 1;
		bmpInfo.bmiHeader.biBitCount = 32;
		bmpInfo.bmiHeader.biCompression = BI_RGB;        
		bmpInfo.bmiHeader.biSizeImage = 0;        
		bmpInfo.bmiHeader.biClrUsed = 0;    
		bmpInfo.bmiHeader.biClrImportant = 0;

		if(0 == GetDIBits(hCaptureDC, hCaptureBitmap, 0,(UINT)miniScreenY, pixel,  (BITMAPINFO *)&bmpInfo, DIB_RGB_COLORS))
		{
			std::cout<<"Receiving Data failed, skipping interation"<<std::endl;
			Sleep(100);
			continue;
		}

		int r,g,b;
		int rB,gB,bB;
		int max1 =0;
		int maxR1=0;
		int maxG1=0;
		int maxB1=0;
		int max2 =0;
		int maxR2=0;
		int maxG2=0;
		int maxB2=0;
		int max3 =0;
		int maxR3=0;
		int maxG3=0;
		int maxB3=0;
		int max4 =0;
		int maxR4=0;
		int maxG4=0;
		int maxB4=0;
		int lastColorIntensity =0;

		memset(histogram,0,sizeof(histogram[0][0][0])*bins*bins*bins);

		for (int i=0; i<miniScreenX*miniScreenY; i++)
		{

			r =GetBValue(pixel[i]);
			g =GetGValue(pixel[i]);
			b =GetRValue(pixel[i]);
			
			rB = (int)(r/255.f*(bins));
			gB = (int)(g/255.f*(bins));
			bB = (int)(b/255.f*(bins));
	
			histogram[rB][gB][bB] ++;
			if(histogram[rB][gB][bB] >=max4)
			{
				int nr = histogram[rB][gB][bB];
				if(rB ==0 &&gB ==0 &&bB ==0 )continue;
				if(nr <max3)
				{
					max4 = nr;
					maxR4 =rB;
					maxG4 =gB;
					maxB4 =bB;
				}
				else if(nr <max2)
				{	
					max3 = nr;
					maxR3 =rB;
					maxG3 =gB;
					maxB3 =bB;
				}
				else if(nr <max1)
				{	
					max2 = nr;
					maxR2 =rB;
					maxG2 =gB;
					maxB2 =bB;
				}
				else
				{	
					max1 = nr;
					maxR1 =rB;
					maxG1 =gB;
					maxB1 =bB;
				}
				
		
			}

		}

		float i1 = (max(maxR1,max(maxG1,maxB1))+min(max(min(maxG1,maxB1),maxR1),max(maxG1,maxB1)))/(float)(maxR1+maxG1+maxB1+1)+0*(maxR1+maxG1+maxB1)/(3*255.f)-((maxR1-maxG1)-(maxR1-maxB1)-(maxG1-maxB1));
		float i2 = (max(maxR2,max(maxG2,maxB2))+min(max(min(maxG2,maxB2),maxR2),max(maxG2,maxB2)))/(float)(maxR2+maxG2+maxB2+1)+0*(maxR2+maxG2+maxB2)/(3*255.f)-((maxR1-maxG1)-(maxR1-maxB1)-(maxG1-maxB1));
		float i3 = (max(maxR3,max(maxG3,maxB3))+min(max(min(maxG3,maxB3),maxR3),max(maxG3,maxB3)))/(float)(maxR3+maxG3+maxB3+1)+0*(maxR3+maxG3+maxB3)/(3*255.f)-((maxR1-maxG1)-(maxR1-maxB1)-(maxG1-maxB1));
		float i4 = (max(maxR4,max(maxG4,maxB4))+min(max(min(maxG4,maxB4),maxR4),max(maxG4,maxB4)))/(float)(maxR4+maxG4+maxB4+1)+0*(maxR4+maxG4+maxB4)/(3*255.f)-((maxR1-maxG1)-(maxR1-maxB1)-(maxG1-maxB1));
	
		float c = max(max(i1,i2),max(i3,i4));
		
		BYTE r2;
		BYTE g2;
		BYTE b2;

		if(c==i1)
		{
		r2 = maxR1/(float)(bins+1)*255;
		g2 = maxG1/(float)(bins+1)*255;
		b2 = maxB1/(float)(bins+1)*255;
		}
		else if(c==i2)
		{
		r2 = maxR2/(float)(bins+1)*255;
		g2 = maxG2/(float)(bins+1)*255;
		b2 = maxB2/(float)(bins+1)*255;
		}
		else if(c ==i3)
		{
		r2 = maxR3/(float)(bins+1)*255;
		g2 = maxG3/(float)(bins+1)*255;
		b2 = maxB3/(float)(bins+1)*255;
		}else if(c==i4)
		{
		r2 = maxR4/(float)(bins+1)*255;
		g2 = maxG4/(float)(bins+1)*255;
		b2 = maxB4/(float)(bins+1)*255;
		}
		
		int r1,g1,b1;
		r1 = lR+(-lR+r2)/12;
		g1 = lG+(-lG+g2)/12;
		b1 = lB+(-lB+b2)/12;
		lR = r1;
		lG= g1;
		lB = b1;
		roccat.Set_LED_RGB(0x01,0x01,0x00,r1,g1,b1);
		
		
	}


	ReleaseDC(hDesktopWnd,hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);


}