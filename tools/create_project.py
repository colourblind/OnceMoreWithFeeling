# Create a new, empty, clean and fresh project

import sys
import os
import shutil
import uuid

def copy_and_replace(in_filename, out_filename, replacements):
    fin = open(in_filename, 'r')
    fout = open(out_filename, 'w')
    for line in fin:
        for i in range(len(replacements)):
            line = line.replace(replacements[i][0], replacements[i][1])
        fout.write(line)
    fin.close()
    fout.close()

def go(project_name):
    path = os.path.join('..', project_name)
    
    # copy project
    shutil.copytree(os.path.join('..', 'Skeleton'), path)
    
    # create project file
    projfile_replacements = [
        (
            '<ProjectGuid>{0ADBD913-B5E4-488E-8B58-D608F71926EB}</ProjectGuid>', 
            '<ProjectGuid>{{{0}}}</ProjectGuid>'.format(str(uuid.uuid4()).upper())
        ),
        ('Skeleton', project_name)
    ]
    copy_and_replace(
        os.path.join(path, 'Skeleton.vcxproj'), 
        os.path.join(path, project_name + '.vcxproj'), 
        projfile_replacements
    )
    os.remove(os.path.join(path, 'Skeleton.vcxproj'))
    
    # update Main.cpp
    os.rename(os.path.join(path, 'Main.cpp'), os.path.join(path, 'Main_s.cpp'))
    copy_and_replace(
        os.path.join(path, 'Main_s.cpp'), 
        os.path.join(path, 'Main.cpp'), 
        [('Skeleton', project_name)]
    )
    os.remove(os.path.join(path, 'Main_s.cpp'))
    
    
if __name__ == '__main__':
    project_name = sys.argv[1]
    go(project_name)
    
