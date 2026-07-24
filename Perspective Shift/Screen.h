#pragma once

namespace Yokoyama
{
	//画面に関する情報の設定
	struct Screen
	{
		static constexpr float WIDTH	= 1280.0f;	  // 画面の幅
		static constexpr float HEIGHT	= 720.0f;	  // 画面の高さ

		static constexpr float TOP		= 0.0f;		  // 画面の上端
		static constexpr float BOTTOM	= HEIGHT;	  // 画面の下端
		static constexpr float LEFT		= 0.0f;		  // 画面の左端
		static constexpr float RIGHT	= WIDTH;	  // 画面の右端
		
		static constexpr float CENTER_X = WIDTH  / 2; // 画面の中心X
		static constexpr float CENTER_Y = HEIGHT / 2; // 画面の中心Y
	}; 
}