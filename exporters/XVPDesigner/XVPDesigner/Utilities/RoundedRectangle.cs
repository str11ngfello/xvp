using System;
using System.Collections.Generic;
using System.Text;
using System.Drawing.Drawing2D;
using System.Drawing;

namespace Utilities
{
    public abstract class RoundedRectangle
    {
        public enum RectangleCorners
        {
            None = 0, TopLeft = 1, TopRight = 2,
            BottomLeft = 4, BottomRight = 8,
            All = TopLeft | TopRight | BottomLeft | BottomRight
        }

		public static GraphicsPath Create(float x, float y, float width,
				 float height, float radius, RectangleCorners corners)
        {
			float xw = x + width;
			float yh = y + height;
			float xwr = xw - radius;
			float yhr = yh - radius;
			float xr = x + radius;
			float yr = y + radius;
			float r2 = radius * 2;
			float xwr2 = xw - r2;
			float yhr2 = yh - r2;

            GraphicsPath p = new GraphicsPath();
            p.StartFigure();

            //Top Left Corner
            if ((RectangleCorners.TopLeft & corners)
                == RectangleCorners.TopLeft)
            {
                p.AddArc(x, y, r2, r2, 180, 90);
            }
            else
            {
                p.AddLine(x, yr, x, y);
                p.AddLine(x, y, xr, y);
            }

            //Top Edge
            p.AddLine(xr, y, xwr, y);

            //Top Right Corner
            if ((RectangleCorners.TopRight & corners)
                == RectangleCorners.TopRight)
            {
                p.AddArc(xwr2, y, r2, r2, 270, 90);
            }
            else
            {
                p.AddLine(xwr, y, xw, y);
                p.AddLine(xw, y, xw, yr);
            }

            //Right Edge
            p.AddLine(xw, yr, xw, yhr);

            //Bottom Right Corner
            if ((RectangleCorners.BottomRight & corners)
                == RectangleCorners.BottomRight)
            {
                p.AddArc(xwr2, yhr2, r2, r2, 0, 90);
            }
            else
            {
                p.AddLine(xw, yhr, xw, yh);
                p.AddLine(xw, yh, xwr, yh);
            }

            //Bottom Edge
            p.AddLine(xwr, yh, xr, yh);

            //Bottom Left Corner
            if ((RectangleCorners.BottomLeft & corners)
                == RectangleCorners.BottomLeft)
            {
                p.AddArc(x, yhr2, r2, r2, 90, 90);
            }
            else
            {
                p.AddLine(xr, yh, x, yh);
                p.AddLine(x, yh, x, yhr);
            }

            //Left Edge
            p.AddLine(x, yhr, x, yr);

            p.CloseFigure();
            return p;
        }

        public static GraphicsPath Create(RectangleF rect,
                 int radius, RectangleCorners corners)
        {
            return Create(rect.X, rect.Y, rect.Width,
                         rect.Height, radius, corners);
        }

		public static GraphicsPath Create(float x, float y, float width,
				 float height, float radius)
        {
            return Create(x, y, width, height, radius,
                         RectangleCorners.All);
        }

		public static GraphicsPath Create(RectangleF rect, float radius)
        {
            return Create(rect.X, rect.Y, rect.Width,
                          rect.Height, radius);
        }

		public static GraphicsPath Create(float x, float y,
				 float width, float height)
        { return Create(x, y, width, height, 5); }

        public static GraphicsPath Create(RectangleF rect)
        { return Create(rect.X, rect.Y, rect.Width, rect.Height); }
    }

}
