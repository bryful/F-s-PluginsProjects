using System;
using System.Text;
using System.IO;
using SkiaSharp;

//string fontPath = "x8y12pxTheStrongGamer.ttf"; // フォントファイルのパス
string fontPath = "x14y24pxHeadUpDaisy.ttf";
int width = 14;
int height = 24; // 8x12ピクセルに合わせるためのサイズ調整
string arrayName = "debug_font_1424";

StringBuilder cppArray = new StringBuilder();
string ss = $"static const unsigned short {arrayName}[][{height}] = {{";
cppArray.AppendLine(ss);

using (var typeface = SKTypeface.FromFile(fontPath))
{
	if (typeface == null)
	{
		Console.WriteLine($"Error: Could not load font from {fontPath}");
		return;
	}

	using (var bmp = new SKBitmap(width * 16, height * 6))
	{
		using (var bmpCanvas = new SKCanvas(bmp))
		{
			bmpCanvas.Clear(SKColors.Black);


			using (var paint = new SKPaint())
			{
				paint.Typeface = typeface;
				paint.TextSize = height;
				paint.Color = SKColors.White;
				paint.IsAntialias = false;
				paint.SubpixelText = false;
				paint.LcdRenderText = false;

				using (var surface = SKSurface.Create(new SKImageInfo(width, height, SKColorType.Rgba8888, SKAlphaType.Premul)))
				{
					var canvas = surface.Canvas;

					// ASCII 0x21 (!) から 0x7E (~) まで
					for (int c = 0x21; c <= 0x7E; c++)
					{
						char character = (char)c;

						canvas.Clear(SKColors.Black);
						// フォントによっては描画位置の微調整が必要
						canvas.DrawText(character.ToString(), 0, 19, paint);

						int ix = (c - 0x21) % 16;
						int iy = (c - 0x21) / 16;

						// surfaceのスナップショットをbmpCanvasに描画
						using (var image = surface.Snapshot())
						{
							bmpCanvas.DrawImage(image, ix * width, iy * height);

							using (var pixmap = image.PeekPixels())
							{
								cppArray.Append("    { ");
								for (int y = 0; y < height; y++)
								{
									ushort rowByte = 0;
									for (int x = 0; x < width; x++)
									{
										// 輝度が一定以上のピクセルを「1」とする
										SKColor pixel = pixmap.GetPixelColor(x, y);
										if (pixel.Red > 128)
										{
											rowByte |= (ushort)(0x8000 >> x);
										}
									}
									cppArray.Append($"0x{rowByte:X4}");
									if (y < height - 1) cppArray.Append(", ");
								}
								cppArray.AppendLine($" }}, // {character}");
							}
						}
						for (int i = 0; i < bmp.Height/height; i++)
						{
							bmpCanvas.DrawLine(
								0, i * height,
								bmp.Width, i * height,
								new SKPaint { Color = SKColors.DarkGray, StrokeWidth = 1 });
						}
						for (int i = 0; i < bmp.Width / width; i++)
						{
							bmpCanvas.DrawLine(
								i*width, 0,
								i * width, bmp.Height,
								new SKPaint { Color = SKColors.DarkGray, StrokeWidth = 1 });
						}
					}
				}
			}
		}
		

		// 全文字をまとめた画像を保存
		using (var stream = File.OpenWrite($"{arrayName}.png"))
		{
			bmp.Encode(stream, SKEncodedImageFormat.Png, 100);
		}
	}
}

cppArray.AppendLine("};");
Console.WriteLine(cppArray.ToString());
