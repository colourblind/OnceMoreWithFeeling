from PIL import Image, ImageFont, ImageDraw

FONT_HEIGHT = 32

font = ImageFont.truetype('arial.ttf', FONT_HEIGHT)
print(font)

image = Image.new('L', (256, 256))
draw = ImageDraw.Draw(image)

x = 0
y = 0
f = open('FontData.h', 'w')

f.write('#pragma once\n')
f.write('\n')
f.write('#include "Fonts.h"\n')
f.write('\n')
f.write('namespace OnceMoreWithFeeling\n')
f.write('{\n')
f.write('    unsigned int FONT_WIDTH = {0};\n'.format(256))
f.write('    unsigned int FONT_HEIGHT = {0};\n'.format(256))
f.write('\n')
f.write('    Glyph FONT_DATA[96] = {\n')

for i in range(32, 128):
    c = str(chr(i))
    s = font.getsize(c)
    
    # the whole FreeTypeFont.getoffset thing does seem to be particularly 
    # documented. Hopefully if doesn't go away
    offset = font.getoffset(c)
    
    if x + s[0] > 256:
        y += FONT_HEIGHT
        x = 0

    draw.text((x, y), c, font=font, fill=255)
    f.write('        Glyph({0}, {1}, {2}, {3}){4}\n'.format(x, y + offset[1], s[0], s[1], ',' if i < 127 else ''))
    
    x += s[0]
    
f.write('    };\n')
f.write('\n')

f.write('    unsigned char FONT_TEXTURE[{0}] = {{'.format(image.size[0] * image.size[1]))
    
count = 0
for x in image.getdata():
    if count % 32 == 0:
        f.write('\n        ')
    f.write('{0}{1} '.format(str(x), ',' if count < 256 * 256 - 1 else ''))
    count = count + 1
    
f.write('\n')
f.write('    };\n')
f.write('}\n')
    
f.close()
image.save('out.png')

print(count)
