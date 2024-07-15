from PIL import Image,ImageDraw
import math

maxi=50

def to_rad(degree):
    return degree*math.pi/180

def my_arc(x, y, r):
    sinus = y/r
    cosinus = x/r
    if cosinus < 0 :
        return math.pi - math.asin(sinus)
    return math.asin(sinus)

def mandelbrot(c):
    z = 0
    j = 0
    while abs(z) <= 2 and j<maxi:
        z = z**2 + c
        j += 1
    return j

(width, height) = (2100, 1400)
(cx, cy, rotationVal, rotationSteps) = (-0.74384935657398, -0.13170134084746293, 90, 10)
(minx, maxx, miny, maxy) = (cx-1.5, cx+1.5, cy-1, cy+1)
rps = rotationVal/rotationSteps

for i in range(1,rotationSteps+1):
    im = Image.new("HSV", (width,height), (0, 0, 0))
    draw = ImageDraw.Draw(im)

    unitx = (maxx-minx)/width
    unity = (maxy-miny)/height

    current_angle = i*rps

    for x in range(width):
        for y in range(height):
            convertedx = minx + x*unitx
            convertedy = miny + y*unity
            r = ((convertedx-cx)**2 + (convertedy-cy)**2)**0.5
            theta = my_arc(convertedx-cx, convertedy-cy, r)
            c = complex(cx+r*math.cos(theta+to_rad(current_angle)), cy+r*math.sin(theta+to_rad(current_angle)))
            hue = int(360*mandelbrot(c)/maxi)
            sat = 100
            if mandelbrot(c) < maxi:
                val = 100
            else:
                val=0
            

            draw.point([x, y], (hue,sat,val))

    im.convert("RGB").save(f"output{i}.png","PNG")
