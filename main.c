#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#define GRID_SIZE 5
#define SCALE 1.0f

typedef struct { float x, y, z; } Vertex;

// Camera
float camX = 0, camY = 5, camZ = 10;
float camYaw = -90, camPitch = -30;
float lastX = 400, lastY = 300;
int firstMouse = 1;

// Mouse callback
void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if(firstMouse){ lastX = xpos; lastY = ypos; firstMouse = 0; }
    float xoff = xpos - lastX;
    float yoff = lastY - ypos;
    lastX = xpos; lastY = ypos;
    float sens = 0.1f; xoff *= sens; yoff *= sens;
    camYaw += xoff; camPitch += yoff;
    if(camPitch > 89) camPitch = 89;
    if(camPitch < -89) camPitch = -89;
}

// Simple wavy height function
float height(float x, float z) { return sinf(x*0.5f)*cosf(z*0.5f)*2.0f; }

// Build 5x5 grid
Vertex* buildGrid(int* vertexCount) {
    int total = GRID_SIZE * GRID_SIZE * 6;
    Vertex* verts = malloc(sizeof(Vertex) * total);
    int idx = 0;
    for(int x = -GRID_SIZE; x < GRID_SIZE; x++) {
        for(int z = -GRID_SIZE; z < GRID_SIZE; z++) {
            float x0 = x*SCALE, x1 = (x+1)*SCALE;
            float z0 = z*SCALE, z1 = (z+1)*SCALE;
            float y00 = height(x0,z0), y10 = height(x1,z0);
            float y01 = height(x0,z1), y11 = height(x1,z1);

            verts[idx++] = (Vertex){x0, y00, z0};
            verts[idx++] = (Vertex){x1, y10, z0};
            verts[idx++] = (Vertex){x0, y01, z1};

            verts[idx++] = (Vertex){x1, y10, z0};
            verts[idx++] = (Vertex){x1, y11, z1};
            verts[idx++] = (Vertex){x0, y01, z1};
        }
    }
    *vertexCount = idx;
    return verts;
}

// Minimal 4x4 matrices
void mat4_identity(float m[16]) { for(int i=0;i<16;i++) m[i]=0; m[0]=m[5]=m[10]=m[15]=1; }

void mat4_perspective(float m[16], float fov, float aspect, float near, float far){
    float f = 1.0f / tanf(fov/2.0f);
    mat4_identity(m);
    m[0] = f/aspect; m[5] = f;
    m[10] = (far+near)/(near-far); m[11] = -1;
    m[14] = (2*far*near)/(near-far); m[15] = 0;
}

void mat4_lookAt(float m[16], float pos[3], float target[3], float up[3]){
    float f[3] = {target[0]-pos[0], target[1]-pos[1], target[2]-pos[2]};
    float len = sqrtf(f[0]*f[0]+f[1]*f[1]+f[2]*f[2]);
    for(int i=0;i<3;i++) f[i]/=len;

    float s[3] = { f[1]*up[2]-f[2]*up[1], f[2]*up[0]-f[0]*up[2], f[0]*up[1]-f[1]*up[0] };
    len = sqrtf(s[0]*s[0]+s[1]*s[1]+s[2]*s[2]);
    for(int i=0;i<3;i++) s[i]/=len;

    float u[3] = { s[1]*f[2]-s[2]*f[1], s[2]*f[0]-s[0]*f[2], s[0]*f[1]-s[1]*f[0] };

    mat4_identity(m);
    m[0]=s[0]; m[4]=s[1]; m[8]=s[2];
    m[1]=u[0]; m[5]=u[1]; m[9]=u[2];
    m[2]=-f[0]; m[6]=-f[1]; m[10]=-f[2];
    m[12]=-(s[0]*pos[0]+s[1]*pos[1]+s[2]*pos[2]);
    m[13]=-(u[0]*pos[0]+u[1]*pos[1]+u[2]*pos[2]);
    m[14]= f[0]*pos[0]+f[1]*pos[1]+f[2]*pos[2];
}

// GLFW error callback
void glfw_error_callback(int error, const char* description){
    fprintf(stderr,"GLFW Error %d: %s\n", error, description);
}

int main() {
    glfwSetErrorCallback(glfw_error_callback);
    if(!glfwInit()){ fprintf(stderr,"GLFW init failed\n"); return -1; }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR,3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR,3);
    glfwWindowHint(GLFW_OPENGL_PROFILE,GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* win = glfwCreateWindow(800,600,"Terrain 5x5",NULL,NULL);
    if(!win){ fprintf(stderr,"Window failed\n"); glfwTerminate(); return -1; }
    glfwMakeContextCurrent(win);

    if(!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)){
        fprintf(stderr,"GLAD failed\n"); return -1;
    }

    glfwSetCursorPosCallback(win, mouse_callback);
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    int vertexCount = 0;
    Vertex* vertices = buildGrid(&vertexCount);

    unsigned int VAO,VBO;
    glGenVertexArrays(1,&VAO); glGenBuffers(1,&VBO);
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER,VBO);
    glBufferData(GL_ARRAY_BUFFER, vertexCount*sizeof(Vertex), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)0);
    glEnableVertexAttribArray(0);

    const char* vertSrc =
        "#version 330 core\n"
        "layout(location=0) in vec3 aPos;\n"
        "uniform mat4 model,view,projection;\n"
        "void main(){gl_Position=projection*view*model*vec4(aPos,1.0);}";
    const char* fragSrc =
        "#version 330 core\n"
        "out vec4 FragColor;\n"
        "void main(){FragColor=vec4(0.2,0.8,0.3,1.0);}";

    unsigned int vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs,1,&vertSrc,NULL); glCompileShader(vs);
    unsigned int fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs,1,&fragSrc,NULL); glCompileShader(fs);

    unsigned int prog = glCreateProgram();
    glAttachShader(prog,vs); glAttachShader(prog,fs); glLinkProgram(prog);
    glDeleteShader(vs); glDeleteShader(fs);

    glEnable(GL_DEPTH_TEST); glClearColor(0.1f,0.1f,0.1f,1.0f);

    float model[16]; mat4_identity(model);
    float proj[16]; mat4_perspective(proj,3.1415f/4.0f,800.0f/600.0f,0.1f,100.0f);

    while(!glfwWindowShouldClose(win)) {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

        float speed = 0.1f;
        if(glfwGetKey(win,GLFW_KEY_W)==GLFW_PRESS) camZ -= speed;
        if(glfwGetKey(win,GLFW_KEY_S)==GLFW_PRESS) camZ += speed;
        if(glfwGetKey(win,GLFW_KEY_A)==GLFW_PRESS) camX -= speed;
        if(glfwGetKey(win,GLFW_KEY_D)==GLFW_PRESS) camX += speed;

        float dir[3] = {
            cosf(camYaw*0.01745f)*cosf(camPitch*0.01745f),
            sinf(camPitch*0.01745f),
            sinf(camYaw*0.01745f)*cosf(camPitch*0.01745f)
        };
        float pos[3] = {camX,camY,camZ};
        float target[3] = {camX+dir[0], camY+dir[1], camZ+dir[2]};
        float up[3] = {0,1,0};
        float view[16]; mat4_lookAt(view,pos,target,up);

        glUseProgram(prog);
        glUniformMatrix4fv(glGetUniformLocation(prog,"model"),1,GL_FALSE,model);
        glUniformMatrix4fv(glGetUniformLocation(prog,"view"),1,GL_FALSE,view);
        glUniformMatrix4fv(glGetUniformLocation(prog,"projection"),1,GL_FALSE,proj);

        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES,0,vertexCount);

        glfwSwapBuffers(win);
        glfwPollEvents();
    }

    free(vertices);
    glfwTerminate();
    return 0;
}