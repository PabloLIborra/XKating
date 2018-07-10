import sys

#receive the file to parse
fileName    = sys.argv[1] #name of the file to read

#open read file and write file
objFile = open(fileName, 'r')

destFile = open("./../Game/media/xml/circuit.xml", 'w')

#################################################################################
#                            TERRAIN                                            #
#################################################################################
TVertexArray = []
TFaceArray = []
TFirstFace = 2147483647
TId = 0

TVertexArray.append(0)   #insert and initial 0 value, in this way we make
                            #that vertex and face numeration matches

#################################################################################
#                            WAYPOINTS                                          #
#################################################################################
WPosArray = []
WRotArray = []
WScaArray = []
WRadius   = []
WLevel     = []
WId       = 10000

#################################################################################
#                            LIGHTS                                             #
#################################################################################
LPosArray = []
LRotArray = []
LScaArray = []
LRadius   = []
LType     = []
LId       = 14000

#################################################################################
#                            ITEM BOX                                           #
#################################################################################
IPosArray = []
IRotArray = []
IScaArray = []
IId       = 16000

#################################################################################
#                            RAMP                                               #
#################################################################################
RPosArray = []
RRotArray = []
RScaArray = []
RMaxVel   = []
RCTime    = []
RDTime    = []
RPoints   = []
RId       = 18000

#################################################################################
#                            OBSTACLES                                          #
#################################################################################
OPosArray = []
ORotArray = []
OScaArray = []
OFile     = []
ORadius   = []
OHeight   = []
OId       = 20000

#################################################################################
#                            START LINE                                         #
#################################################################################
SPosArray = []
SRotArray = []
SScaArray = []
SPoints   = []
SId       = 24000

#################################################################################
#                       OTHER VARIABLES                                         #
#################################################################################

#Variables for knowing when an object has been fully analyzed
WVisited   = False
LVisited   = False 
IVisited   = False
RVisited   = 0
OVisited   = False
SVisited   = 0

#Variables for each different object
Terrain     = False
Waypoint    = False
Light       = False 
ItemBox     = False
Ramp        = False
Object      = False
StartLine   = False

#read and store data from obj file
for line in objFile:
        #Here we detect which type of object we are reading
    if line[0] == 'o' :
        Terrain     = False
        Waypoint    = False
        Light       = False 
        ItemBox     = False
        Ramp        = False
        Object      = False
        StartLine   = False

        WVisited   = False
        LVisited   = False 
        IVisited   = False
        RVisited   = False
        OVisited   = False
        SVisited   = False

        if line[2] == 'T' :
            Terrain         = True
        if line[2] == 'W' :
            Waypoint        = True
        if line[2] == 'L' :
            Light           = True
        if line[2] == 'I' :
            ItemBox         = True
        if line[2] == 'R' :
            Ramp            = True
        if line[2] == 'O' :
            Object          = True
        if line[2] == 'S' :
            StartLine       = True

    #################################################################################
    #                            TERRAIN                                            #
    #################################################################################
    if Terrain == True and line[0] == 'v' and line[1] == ' ' :

        #split vertex into x,y,z and push them into vertex array
        aux = line.split(' ')
        vec3 = []
        vec3.append(float(aux[1]))
        vec3.append(float(aux[2]))
        vec3.append(float(aux[3]))
        TVertexArray.append(vec3)
    elif Terrain == True and line[0] == 'f' and line[1] == ' ' :
        aux = line.split(' ')
        vec4 = []
        vec4.append(int(aux[1].split('/')[0]))
        vec4.append(int(aux[2].split('/')[0]))
        vec4.append(int(aux[3].split('/')[0]))
        vec4.append(int(aux[4].split('/')[0]))
        TFaceArray.append(vec4)

        #get the first face in order to index correctly
        if TFirstFace > int(aux[1].split('/')[0]) - 1 :
            TFirstFace = int(aux[1].split('/')[0]) - 1
        if TFirstFace > int(aux[2].split('/')[0]) - 1 :
            TFirstFace = int(aux[2].split('/')[0]) - 1
        if TFirstFace > int(aux[3].split('/')[0]) - 1 :
            TFirstFace = int(aux[3].split('/')[0]) - 1
        if TFirstFace > int(aux[4].split('/')[0]) - 1 :
            TFirstFace = int(aux[4].split('/')[0]) - 1

    #################################################################################
    #                            WAYPOINT                                           #
    #################################################################################
    if Waypoint == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        #Set Waipoint Level
        WLevel.append(aux[1])

        #Set Radius of Waypoint
        WRadius.append(aux[2])

        #Set Rotation if given, and 0 if not
        if len(aux) > 4 :
            WRotArray.append(aux[3].split(':')[0] + ',' + aux[3].split(':')[1] + ',' + aux[3].split(':')[2])
        else :
            WRotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 5 :
            WScaArray.append(aux[4].split(':')[0] + ',' + aux[4].split(':')[1] + ',' + aux[4].split(':')[2])
        else :
            WScaArray.append('1,1,1')

    #Here we store data for the position of our LIGHT
    elif Waypoint == True and WVisited == False and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')
        WPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        WVisited = True

    #################################################################################
    #                            LIGHT                                              #
    #################################################################################
    if Light == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        #Check Type of Light
        if aux[2][0] == 'P' :
            LType.append('P')
        if aux[2][0] == 'D' :
            LType.append('D')

        #Set Radius of Light
        LRadius.append(aux[3])

        #Set Rotation if given, and 0 if not
        if len(aux) > 5 :
            LRotArray.append(aux[4].split(':')[0] + ',' + aux[4].split(':')[1] + ',' + aux[4].split(':')[2])
        else :
            LRotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 6 :
            LScaArray.append(aux[5].split(':')[0] + ',' + aux[5].split(':')[1] + ',' + aux[5].split(':')[2])
        else :
            LScaArray.append('1,1,1')

    #Here we store data for the position of our LIGHT
    elif Light == True and LVisited == False and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')
        LPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        LVisited = True

    #################################################################################
    #                            ITEM BOX                                           #
    #################################################################################
    if ItemBox == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        #Set Rotation if given, and 0 if not
        if len(aux) > 3 :
            IRotArray.append(aux[2].split(':')[0] + ',' + aux[2].split(':')[1] + ',' + aux[2].split(':')[2])
        else :
            IRotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 4 :
            IScaArray.append(aux[3].split(':')[0] + ',' + aux[3].split(':')[1] + ',' + aux[3].split(':')[2])
        else :
            IScaArray.append('1,1,1')

    #Here we store data for the position of our LIGHT
    elif ItemBox == True and IVisited == False and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')
        IPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        IVisited = True

    #################################################################################
    #                            RAMP                                               #
    #################################################################################
    if Ramp == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        #Set Velocity of Ramp
        RMaxVel.append(aux[2])
        RCTime.append(aux[3])
        RDTime.append(aux[4])

        #Set Rotation if given, and 0 if not
        if len(aux) > 6 :
            RRotArray.append(aux[5].split(':')[0] + ',' + aux[5].split(':')[1] + ',' + aux[5].split(':')[2])
        else :
            RRotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 7 :
            RScaArray.append(aux[6].split(':')[0] + ',' + aux[6].split(':')[1] + ',' + aux[6].split(':')[2])
        else :
            RScaArray.append('1,1,1')

        RVisited = 0

    #Here we store data for the position of our LIGHT
    elif Ramp == True and RVisited < 4 and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')

        if RVisited == 0 :
            RPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        RPoints.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        if RVisited == 3 :
            aux1 = RPoints[2]
            RPoints[2] = RPoints[3]
            RPoints[3] = aux1

        RVisited += 1

    #################################################################################
    #                            OBSTACLES                                          #
    #################################################################################
    if Object == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        OFile.append(aux[2])
        ORadius.append(aux[3])
        OHeight.append(aux[4])

        #Set Rotation if given, and 0 if not
        if len(aux) > 6 :
            ORotArray.append(aux[5].split(':')[0] + ',' + aux[5].split(':')[1] + ',' + aux[5].split(':')[2])
        else :
            ORotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 7 :
            OScaArray.append(aux[6].split(':')[0] + ',' + aux[6].split(':')[1] + ',' + aux[6].split(':')[2])
        else :
            OScaArray.append('1,1,1')

    #Here we store data for the position of our LIGHT
    elif Object == True and OVisited == False and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')
        OPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        OVisited = True

    #################################################################################
    #                            START LINE                                         #
    #################################################################################
    if StartLine == True and line[0] == 'o' and line[1] == ' ' :
        aux = line.split(' ')[1].split('_')

        #Set Rotation if given, and 0 if not
        if len(aux) > 3 :
            SRotArray.append(aux[2].split(':')[0] + ',' + aux[2].split(':')[1] + ',' + aux[2].split(':')[2])
        else :
            SRotArray.append('0,0,0')


        #Set Scale if given, and 0 if not
        if len(aux) > 4 :
            SScaArray.append(aux[3].split(':')[0] + ',' + aux[3].split(':')[1] + ',' + aux[3].split(':')[2])
        else :
            SScaArray.append('1,1,1')

        SVisited = 0

    #Here we store data for the position of our LIGHT
    elif StartLine == True and SVisited < 4 and line[0] == 'v' and line[1] == ' ' :

        aux = line.split(' ')

        if SVisited == 0 :
            SPosArray.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        SPoints.append(aux[1] + ',' + aux[2] + ',' + aux[3].rstrip())

        SVisited += 1

#close obj file once read
objFile.close()


#Extra MATHS for TERRAIN
#reorder vertex in faces depending on its x and z values
for i, face in enumerate(TFaceArray):

    min_x = 2147483647
    max_z = -2147483647
    nFace = 0

    for vertex in face:
        
        #Get correct index
        vtx = vertex-TFirstFace

        if min_x >= TVertexArray[vtx][0]: #check for the value that is most in the left

            if min_x == TVertexArray[vtx][0]: #if another point has the same x
                if max_z < TVertexArray[vtx][2]: #go for the higher z
                    min_x = TVertexArray[vtx][0]
                    max_z = TVertexArray[vtx][2]
                    nFace = vertex 
            else:
                min_x = TVertexArray[vtx][0]  #if it's the lower x, save the value
                max_z = TVertexArray[vtx][2]
                nFace = vertex
            
    #once we have the starting vertex, we rotate each vector to have 
    #the starting vertex in its start

    #firstly we search for the position in with it's flaced the starting face
    faceStart = 0
    for faceStart, vertex in enumerate(face):
        if nFace == vertex:
            break

    #then we loop over the array reallocating the data
    auxFace = []
    for idx in range(0,4):
        auxFace.append(face[faceStart])
        if faceStart == 3:
            faceStart = 0
        else:
            faceStart += 1

    #then we reassign the face
    TFaceArray[i] = auxFace

#make an array that holds for every face, which other faces have vertex in common with it
relatedFaces = [None] * len(TFaceArray)
for i, iter in enumerate(relatedFaces):
    relatedFaces[i] = []    #create empty 2D array
for i, face in enumerate(TFaceArray):    #we iterate for every face with all other faces
    for j, oFace in enumerate(TFaceArray):
        if i != j:                      #if we aren't comparing with the same plane 
            occurence = 0
            for vertex in TFaceArray[i]:  #we compare every vertex of our face to the vertex of the other faces
                if vertex == TFaceArray[j][0]  or vertex == TFaceArray[j][1] or vertex == TFaceArray[j][2] or vertex == TFaceArray[j][3]:
                    occurence += 1
            if occurence > 1:
                    relatedFaces[i].append(j) #if there's a vertex in common, add j

#for every edge of every face, mark connected edges or -1 if none
relatedEdges = [None] * len(TFaceArray)
for i, iter in enumerate(relatedEdges):
    relatedEdges[i] = [str(-1),str(-1),str(-1),str(-1)]
for i, face in enumerate(relatedFaces): #for every face
    for faceId in face: #check faces connecting with it
        for j in range(0, 4):    #for every line of our face
            aux = j + 1 
            if aux == 4:
                aux = 0
                
            for z in range(0, 4):   #compare with all the other four lines
                aux1 = z + 1 
                if aux1 == 4:
                    aux1 = 0

                #if a pair of points is related to another
                if(TFaceArray[i][j] == TFaceArray[faceId][z] or TFaceArray[i][j] == TFaceArray[faceId][aux1]) and \
                  (TFaceArray[i][aux] == TFaceArray[faceId][z] or TFaceArray[i][aux] == TFaceArray[faceId][aux1]):
                    relatedEdges[i][j] = str(TId+faceId) + ':' + str(z)

#Extra MATHS for RAMP
for i, iter in enumerate(RPosArray):

    x = float(RPoints[i*4].split(',')[0]) + float(RPoints[i*4+1].split(',')[0]) + float(RPoints[i*4+2].split(',')[0]) + float(RPoints[i*4+3].split(',')[0])
    y = float(RPoints[i*4].split(',')[1]) + float(RPoints[i*4+1].split(',')[1]) + float(RPoints[i*4+2].split(',')[1]) + float(RPoints[i*4+3].split(',')[1])
    z = float(RPoints[i*4].split(',')[2]) + float(RPoints[i*4+1].split(',')[2]) + float(RPoints[i*4+2].split(',')[2]) + float(RPoints[i*4+3].split(',')[2])

    x /= 4
    y /= 4
    z /= 4

    RPosArray[i] = str(x) + ',' + str(y) + ',' + str(z)

#Extra MATHS for START LINE
for i, iter in enumerate(SPosArray):

    x = float(SPoints[i*4].split(',')[0]) + float(SPoints[i*4+1].split(',')[0]) + float(SPoints[i*4+2].split(',')[0]) + float(SPoints[i*4+3].split(',')[0])
    y = float(SPoints[i*4].split(',')[1]) + float(SPoints[i*4+1].split(',')[1]) + float(SPoints[i*4+2].split(',')[1]) + float(SPoints[i*4+3].split(',')[1])
    z = float(SPoints[i*4].split(',')[2]) + float(SPoints[i*4+1].split(',')[2]) + float(SPoints[i*4+2].split(',')[2]) + float(SPoints[i*4+3].split(',')[2])

    x /= 4
    y /= 4
    z /= 4

    SPosArray[i] = str(x) + ',' + str(y) + ',' + str(z)

################################################################################
#                       Write the Xml                                          #
################################################################################
#TERRAIN
for i, face in enumerate(TFaceArray):

    p1 = 'p1=\"' + str(TVertexArray[face[0]-TFirstFace][0]) + ',' + str(TVertexArray[face[0]-TFirstFace][1]) + ',' + str(TVertexArray[face[0]-TFirstFace][2]) + '\" '
    p2 = 'p2=\"' + str(TVertexArray[face[1]-TFirstFace][0]) + ',' + str(TVertexArray[face[1]-TFirstFace][1]) + ',' + str(TVertexArray[face[1]-TFirstFace][2]) + '\" '
    p3 = 'p3=\"' + str(TVertexArray[face[2]-TFirstFace][0]) + ',' + str(TVertexArray[face[2]-TFirstFace][1]) + ',' + str(TVertexArray[face[2]-TFirstFace][2]) + '\" ' 
    p4 = 'p4=\"' + str(TVertexArray[face[3]-TFirstFace][0]) + ',' + str(TVertexArray[face[3]-TFirstFace][1]) + ',' + str(TVertexArray[face[3]-TFirstFace][2]) + '\" '

    pos = '\"' + str(TVertexArray[face[0]-TFirstFace][0]) + ',' + str(TVertexArray[face[0]-TFirstFace][1]) + ',' + str(TVertexArray[face[0]-TFirstFace][2]) + '\" '
    p0  = '\"' + str(0) + ',' + str(0) + ',' + str(0) + '\" '

    gameObject          =  '<object id=\"' + str(TId+i) + '\" pos=' + pos + ' rot=' + p0 + ' sca=' + p0 + '>\n'
    terrainComponent    =  '    <component name=\"terrain\"' + ' l0=\"' + relatedEdges[i][0] + '\" l1=\"' + relatedEdges[i][1] + '\" l2=\"' + relatedEdges[i][2] + '\" l3=\"' + relatedEdges[i][3] + '\">\n'
    bbox                =  '        <bbox ' + p1 + p2 + p3 + p4 + 'friction=\"0.2\" />\n'
    terrainComponentEnd =  '    </component>\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(terrainComponent)
    destFile.write(bbox)
    destFile.write(terrainComponentEnd)
    destFile.write(gameObjectEnd)

#WAYPOINT
for i in range( 0, len(WPosArray) ) :

    gameObject          =  '<object id=\"' + str(WId+i) + '\" pos=\"' + WPosArray[i] + '\" rot=\"' + WRotArray[i] + '\" sca=\"' + WScaArray[i] + '\">\n'
    waypointComponent   =  '    <component name=\"waypoint\"' + ' radius=\"' + WRadius[i] + '\" level=\"' + WLevel[i] + '\" />\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(waypointComponent)
    destFile.write(gameObjectEnd)

#LIGHT
for i in range( 0, len(LPosArray) ) :

    gameObject          =  '<object id=\"' + str(LId+i) + '\" pos=\"' + LPosArray[i] + '\" rot=\"' + LRotArray[i] + '\" sca=\"' + LScaArray[i] + '\">\n'
    lightComponent      =  '    <component name=\"light\"' + ' radius=\"' + LRadius[i] + '\" type=\"' + LType[i] + '\" />\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(lightComponent)
    destFile.write(gameObjectEnd)

#ITEM BOX
for i in range( 0, len(IPosArray) ) :

    gameObject          =  '<object id=\"' + str(IId+i) + '\" pos=\"' + IPosArray[i] + '\" rot=\"' + IRotArray[i] + '\" sca=\"' + IScaArray[i] + '\">\n'
    itemComponent       =  '    <component name=\"itemBox\"' + ' />\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(itemComponent)
    destFile.write(gameObjectEnd)

#RAMP
for i in range( 0, len(RPosArray) ) :

    gameObject          =  '<object id=\"' + str(RId+i) + '\" pos=\"' + RPosArray[i] + '\" rot=\"' + RRotArray[i] + '\" sca=\"' + RScaArray[i] + '\">\n'
    renderComponent     =  '    <component name=\"render\"' + ' file=\"ramp.jpg\" type=\"plane\" />\n'
    collisionComponent  =  '    <component name=\"collision\" kinetic=\"false\" type=\"ramp\">\n'
    bbox                =  '        <bbox p1=\"' + RPoints[i*4] + '\" p2=\"' + RPoints[i*4+1] + '\" p3=\"' + RPoints[i*4+3] + '\" p4=\"' + RPoints[i*4+2] + '\" ' + 'friction=\"0.2\" />\n'
    collisiComponentEnd =  '    </component>\n'
    rampComponent       =  '    <component name=\"ramp\"' + ' vel=\"' + RMaxVel[i] + '\" cTime=\"' + RCTime[i] + '\" dTime=\"' + RDTime[i] +'\" />\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(renderComponent)
    destFile.write(collisionComponent)
    destFile.write(bbox)
    destFile.write(collisiComponentEnd)
    destFile.write(rampComponent)
    destFile.write(gameObjectEnd)

#OBSTACLE
for i in range( 0, len(OPosArray) ) :

    gameObject          =  '<object id=\"' + str(OId+i) + '\" pos=\"' + OPosArray[i] + '\" rot=\"' + ORotArray[i] + '\" sca=\"' + OScaArray[i] + '\">\n'
    renderComponent     =  '    <component name=\"render\"' + ' file=\"' + OFile[i] + '\" type=\"mesh\" />\n'
    collisionComponent  =  '    <component name=\"collision\" radius=\"' + ORadius[i] + '\" height=\"' + OHeight[i] + '\" kinetic=\"true\" type=\"default\" />\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(renderComponent)
    if ORadius[i] != "0" :
        destFile.write(collisionComponent)
    destFile.write(gameObjectEnd)

#START LINE
for i in range( 0, len(SPosArray) ) :

    gameObject          =  '<object id=\"' + str(SId+i) + '\" pos=\"' + SPosArray[i] + '\" rot=\"' + SRotArray[i] + '\" sca=\"' + SScaArray[i] + '\">\n'
    renderComponent     =  '    <component name=\"render\"' + ' file=\"starLine.png\" type=\"plane\" />\n'
    collisionComponent  =  '    <component name=\"collision\" kinetic=\"false\" type=\"start\">\n'
    bbox                =  '        <bbox p1=\"' + SPoints[i*4] + '\" p2=\"' + SPoints[i*4+1] + '\" p3=\"' + SPoints[i*4+3] + '\" p4=\"' + SPoints[i*4+2] + '\" ' + 'friction=\"0.2\" />\n'
    collisiComponentEnd =  '    </component>\n'
    gameObjectEnd       =  '</object>\n'

    destFile.write(gameObject)
    destFile.write(renderComponent)
    destFile.write(collisionComponent)
    destFile.write(bbox)
    destFile.write(collisiComponentEnd)
    destFile.write(gameObjectEnd)

#close write file
destFile.close()

