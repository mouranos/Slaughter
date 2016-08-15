#include <codecvt>
#include <locale>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "character/enemy.h"
#include "character/player.h"
#include "game.h"
#include "util/bulletgenobj.h"
#include "util/controls.h"
#include "util/objloader.h"
#include "util/shader.h"
#include "util/textrenderer.h"
#include "util/texture.h"
#include "util/vboindexer.h"
#include "util/windowcreater.h"

constexpr unsigned int WIDTH = 1440;
constexpr unsigned int HEIGHT = 864;
constexpr unsigned int TIME_LIMIT = 90;

bool startGame()
{
    Window window(WIDTH, HEIGHT, "開成祭 FPS-main");

    glfwSetInputMode(window, GLFW_STICKY_KEYS, GL_TRUE);
    //    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glClearColor(135.f / 256, 206.f / 256, 235.f / 256, 0.f);

    // Initialize dynamics world
    DynamicsWorld dynamicsWorld;

    // World's shader
    GLuint worldProgramID = LoadShaders("shader/mainVertexShader.glsl",
                                        "shader/mainFragmentShader.glsl");

    // World's matrices
    GLuint worldProjectionMatrixID = glGetUniformLocation(worldProgramID, "P");
    GLuint worldViewMatrixID = glGetUniformLocation(worldProgramID, "V");

    // Ground's matrices
    GLuint groundModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    // Ground's IDs
    GLuint groundVertexPositionModelspaceID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint groundVertexUvID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint groundTextureID =
        glGetUniformLocation(worldProgramID, "myTextureSampler");

    // Ground's data
    float halfGroundSize = 600.f;
    const unsigned int splitNumber = 6;
    GLfloat groundVertexBufferData[splitNumber * splitNumber][3 * 4];
    for(int i = 0; i < splitNumber * splitNumber; i++)
    {
        groundVertexBufferData[i][0] =
            -halfGroundSize + halfGroundSize * 2 / splitNumber * (i % 6);
        groundVertexBufferData[i][1] = 0;
        groundVertexBufferData[i][2] =
            -halfGroundSize + halfGroundSize * 2 / splitNumber * (i / 6);
        groundVertexBufferData[i][3] = groundVertexBufferData[i][0];
        groundVertexBufferData[i][4] = 0;
        groundVertexBufferData[i][5] =
            groundVertexBufferData[i][2] + halfGroundSize * 2 / splitNumber;
        groundVertexBufferData[i][6] =
            groundVertexBufferData[i][0] + halfGroundSize * 2 / splitNumber;
        groundVertexBufferData[i][7] = 0;
        groundVertexBufferData[i][8] =
            groundVertexBufferData[i][2] + halfGroundSize * 2 / splitNumber;
        groundVertexBufferData[i][9] =
            groundVertexBufferData[i][0] + halfGroundSize * 2 / splitNumber;
        groundVertexBufferData[i][10] = 0;
        groundVertexBufferData[i][11] = groundVertexBufferData[i][2];
    }

    const GLfloat groundUvBufferData[] = {0.f, 0.f, 1.f, 0.f,
                                          1.f, 1.f, 0.f, 1.f};

    GLuint groundTexture =
        loadBMP_custom("material/building/ground/groundTexture2.bmp");

    // Ground's buffers
    GLuint groundVertexBuffer[splitNumber * splitNumber];
    glGenBuffers(splitNumber * splitNumber, groundVertexBuffer);
    for(int i : groundVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, i);
        glBufferData(GL_ARRAY_BUFFER, sizeof(groundVertexBufferData[i]),
                     groundVertexBufferData[i], GL_STATIC_DRAW);
    }

    GLuint groundUvBuffer;
    glGenBuffers(splitNumber, &groundUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, groundUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(groundUvBufferData),
                 groundUvBufferData, GL_STATIC_DRAW);


    GLfloat wallHeight = 100;
    dynamicsWorld.addRigidBody(BOX, btVector3(halfGroundSize,wallHeight,1.f),0,btVector3(0,0,0),btVector3(0,0,halfGroundSize+1),btVector3(0,1,0),0.f,0.f, 0.7f);
    dynamicsWorld.addRigidBody(BOX, btVector3(halfGroundSize,wallHeight,1.f),0,btVector3(0,0,0),btVector3(0,0,-halfGroundSize-1),btVector3(0,1,0),0.f,0.f, 0.7f);
    dynamicsWorld.addRigidBody(BOX, btVector3(halfGroundSize,wallHeight,1.f),0,btVector3(0,0,0),btVector3(halfGroundSize+1, 0,0),btVector3(0,1,0),btRadians(90.f),0.f, 0.7f);
    dynamicsWorld.addRigidBody(BOX, btVector3(halfGroundSize,wallHeight,1.f),0,btVector3(0,0,0),btVector3(-halfGroundSize-1,0,0),btVector3(0,1,0),btRadians(90.f),0.f, 0.7f);


    GLuint wallModelMatrixID = glGetUniformLocation(worldProgramID, "M");
    GLuint wallVertexPositionID = glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint wallVertexUvID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint wallTextureID = glGetUniformLocation(worldProgramID, "myTextureSampler");

    GLuint wallTexture = loadImage("material/building/wall/wallTexture.png");

    GLfloat wallVertexBufferData[4][12] = {
        {-halfGroundSize, 0, -halfGroundSize, -halfGroundSize, wallHeight,
         -halfGroundSize, -halfGroundSize, wallHeight, halfGroundSize,
         -halfGroundSize, 0, halfGroundSize},
        {-halfGroundSize, 0, halfGroundSize, -halfGroundSize, wallHeight,
         halfGroundSize, halfGroundSize, wallHeight, halfGroundSize,
         halfGroundSize, 0, halfGroundSize},
        {halfGroundSize, 0, halfGroundSize, halfGroundSize, wallHeight,
         halfGroundSize, halfGroundSize, wallHeight, -halfGroundSize,
         halfGroundSize, 0, -halfGroundSize},
        {halfGroundSize, 0, -halfGroundSize, halfGroundSize, wallHeight,
         -halfGroundSize, -halfGroundSize, wallHeight, -halfGroundSize,
         -halfGroundSize, 0, -halfGroundSize}};

    GLfloat wallUvBufferData[] = {0.f, 1.f, 0.f, 0.f, 1.f, 0.f, 1.f, 1.f};

    GLuint wallVertexBuffers[4];
    glGenBuffers(4, wallVertexBuffers);
    for(int i = 0; i < 4; i++)
    {
    glBindBuffer(GL_ARRAY_BUFFER, wallVertexBuffers[i]);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(wallVertexBufferData),
                 wallVertexBufferData[i],
                 GL_STATIC_DRAW);
    }

    GLuint wallUvBuffer;
    glGenBuffers(1, &wallUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, wallUvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(wallUvBufferData),
                 wallUvBufferData, GL_STATIC_DRAW);

    // Configure characters
    Player player;
    auto playerBodyItr = dynamicsWorld.addRigidBody(
        CAPSULE, player.getCharBody().getHalfExtents(),
        player.getCharBody().getMass(), btVector3(0, 0, 0), btVector3(0, 20, 0),
        btVector3(0, 1, 0), 0, 0.9f, 0.7f);
    ComputeMatrices worldMatrices(window, player, playerBodyItr);

    unsigned int numEnemies = 4;
    btVector3 enemySpawnPoint[] = {
        btVector3(100, 40, 100), btVector3(100, 40, -100),
        btVector3(-100, 40, 100), btVector3(-100, 40, -100)};
    Enemy enemies[numEnemies];
    std::list<DynamicsWorld::ObjectData>::iterator enemyBodyItrs[numEnemies];
    for(int i = 0; i < numEnemies; i++)
    {
        enemyBodyItrs[i] = dynamicsWorld.addRigidBody(
            CAPSULE, enemies[i].getCharBody().getHalfExtents(),
            enemies[i].getCharBody().getMass(), btVector3(0, 0, 0),
            enemySpawnPoint[i], btVector3(0, 1, 0), 0, 0.8f, 0.7f);
    }

    loadOBJ enemyObj("material/monster/buba.obj");
    GLuint enemyTexture = loadBMP_custom("material/monster/buba_tex.bmp");

    GLuint enemyModelMatrixID = glGetUniformLocation(worldProgramID, "M");
    GLuint enemyVertexPositionID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint enemyVertexUvID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint enemyTextureID =
        glGetUniformLocation(worldProgramID, "myTextureSampler");

    GLuint enemyVertexBuffer[numEnemies];
    glGenBuffers(numEnemies, enemyVertexBuffer);
    for(int i : enemyVertexBuffer)
    {
        glBindBuffer(GL_ARRAY_BUFFER, i);
        glBufferData(GL_ARRAY_BUFFER,
                     enemyObj.getIndexedVertices().size() * sizeof(glm::vec3),
                     enemyObj.getIndexedVertices().data(), GL_STATIC_DRAW);
    }

    GLuint enemyUvBuffer;
    glGenBuffers(1, &enemyUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, enemyUvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 enemyObj.getIndexedUvs().size() * sizeof(glm::vec2),
                 enemyObj.getIndexedUvs().data(), GL_STATIC_DRAW);

    GLuint enemyElementBuffer;
    glGenBuffers(1, &enemyElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enemyElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 enemyObj.getIndices().size() * sizeof(unsigned short),
                 enemyObj.getIndices().data(), GL_STATIC_DRAW);

    // m16's matrices
    GLuint m16ProjectionMatrixID = glGetUniformLocation(worldProgramID, "P");
    GLuint m16ViewMatrixID = glGetUniformLocation(worldProgramID, "V");
    GLuint m16ModelMatrixID = glGetUniformLocation(worldProgramID, "M");

    // m16's MVP
    glm::mat4 m16ProjectionMatrix =
        glm::perspective(45.f, 4.f / 3.f, 0.1f, 100.f); // TODO: set cliping
    glm::mat4 m16ViewMatrix;
    glm::mat4 m16ModelMatrix = glm::scale(glm::vec3(0.1, 0.1, 0.1));

    // m16's IDs
    GLuint m16VertexPositionModelspaceID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint m16VertexUvID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint m16TextureID =
        glGetUniformLocation(worldProgramID, "myTextureSampler");

    // m16's data
    loadOBJ m16Obj("material/gun/m16/m16.obj");
    GLuint m16Texture = loadBMP_custom("material/gun/m16/m16.bmp");

    // m16's buffers
    GLuint m16VertexBuffer;
    glGenBuffers(1, &m16VertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16VertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 m16Obj.getIndexedVertices().size() * sizeof(glm::vec3),
                 m16Obj.getIndexedVertices().data(), GL_STATIC_DRAW);

    GLuint m16UvBuffer;
    glGenBuffers(1, &m16UvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, m16UvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 m16Obj.getIndexedUvs().size() * sizeof(glm::vec2),
                 m16Obj.getIndexedUvs().data(), GL_STATIC_DRAW);

    GLuint m16ElementBuffer;
    glGenBuffers(1, &m16ElementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16ElementBuffer);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 m16Obj.getIndices().size() * sizeof(unsigned short),
                 m16Obj.getIndices().data(), GL_STATIC_DRAW);

    // Ball
    // Ball's data
    loadOBJ ballObj("material/shape/sphere/webtrcc.obj");
    GLuint ballTexture = loadBMP_custom("material/shape/sphere/leather.bmp");

    // Ball's IDs
    GLuint ballModelMatrixID = glGetUniformLocation(worldProgramID, "M");
    GLuint ballVertexPositionID =
        glGetAttribLocation(worldProgramID, "vertexPosition_modelspace");
    GLuint ballVertexUvID = glGetAttribLocation(worldProgramID, "vertexUV");
    GLuint ballTextureID =
        glGetUniformLocation(worldProgramID, "myTextureSampler");

    // Ball's buffers
    GLuint ballVertexBuffer;
    glGenBuffers(1, &ballVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedVertices().size() * sizeof(glm::vec3),
                 ballObj.getIndexedVertices().data(), GL_STATIC_DRAW);

    GLuint ballUvBuffer;
    glGenBuffers(1, &ballUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballUvBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedUvs().size() * sizeof(glm::vec2),
                 ballObj.getIndexedUvs().data(), GL_STATIC_DRAW);

    GLuint ballNormalBuffer;
    glGenBuffers(1, &ballNormalBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballNormalBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndexedNormals().size() * sizeof(glm::vec3),
                 ballObj.getIndexedNormals().data(), GL_STATIC_DRAW);

    GLuint ballElementBuffer;
    glGenBuffers(1, &ballElementBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, ballElementBuffer);
    glBufferData(GL_ARRAY_BUFFER,
                 ballObj.getIndices().size() * sizeof(unsigned short),
                 ballObj.getIndices().data(), GL_STATIC_DRAW);

    auto ballItr = dynamicsWorld.addRigidBody(
        SPHERE, btVector3(10.f, 0.f, 0.f), 15.f, btVector3(0.f, 0.f, 0.f),
        btVector3(0.f, 30.f, 0.f), btVector3(0.f, 1.f, 0.f), btRadians(0.f),
        0.8f, 0.7f);

    double timeBeforeLoop = glfwGetTime();
    std::wstring_convert<std::codecvt_utf8<char32_t>, char32_t> converter;
    std::u32string timeLimit = converter.from_bytes(
        std::to_string(TIME_LIMIT - static_cast<int>(timeBeforeLoop)).data());
    unsigned int textSize = 128;
    RenderText timeText(timeLimit, textSize);

    float halfAimSize = 100;
    const GLfloat aimVertexBufferData[] = {
        -halfAimSize + WIDTH,  -halfAimSize + HEIGHT, halfAimSize + WIDTH,
        -halfAimSize + HEIGHT, halfAimSize + WIDTH,   halfAimSize + HEIGHT,
        -halfAimSize + WIDTH,  halfAimSize + HEIGHT};
    const GLint aimUvBufferData[] = {0, 0, 1, 0, 1, 1, 0, 1};

    GLuint aimProgramID = LoadShaders("shader/2DVertexShader.glsl",
                                      "shader/2DFragmentShader.glsl");
    GLuint aimTexture = loadImage("material/others/aim.png");
    GLuint aimVertexPositionID =
        glGetAttribLocation(aimProgramID, "vertexPosition_screenspace");
    GLuint aimVertexUvID = glGetAttribLocation(aimProgramID, "vertexUV");
    GLuint aimTextureID =
        glGetUniformLocation(aimProgramID, "myTextureSampler");

    GLuint aimVertexBuffer;
    glGenBuffers(1, &aimVertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, aimVertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aimVertexBufferData),
                 aimVertexBufferData, GL_STATIC_DRAW);

    GLuint aimUvBuffer;
    glGenBuffers(1, &aimUvBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, aimUvBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(aimUvBufferData), aimUvBufferData,
                 GL_STATIC_DRAW);

    double lastTime = glfwGetTime();
    do
    {
        double deltaTime = getDeltaTime(&lastTime);
        dynamicsWorld.getDynamicsWorld().stepSimulation(deltaTime);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);
        //        glEnable(GL_CULL_FACE);
        //        glCullFace(GL_BACK);
        glDepthFunc(GL_LESS);

        glUseProgram(worldProgramID);

        // Draw world
        // Draw field
        worldMatrices.computeMatricesFromInputs(deltaTime);

        glm::mat4 worldProjectionMatrix = worldMatrices.getProjectionMatrix();
        glm::mat4 worldViewMatrix = worldMatrices.getViewMatrix();


        glUniformMatrix4fv(worldProjectionMatrixID, 1, GL_FALSE,
                           &worldProjectionMatrix[0][0]);
        glUniformMatrix4fv(worldViewMatrixID, 1, GL_FALSE,
                           &worldViewMatrix[0][0]);

        glm::mat4 groundModelMatrix(1.f);

        glUniformMatrix4fv(groundModelMatrixID, 1, GL_FALSE,
                           &groundModelMatrix[0][0]);

        for(int i : groundVertexBuffer)
        {
            glEnableVertexAttribArray(groundVertexPositionModelspaceID);
            glBindBuffer(GL_ARRAY_BUFFER, i);
            glVertexAttribPointer(groundVertexPositionModelspaceID, 3, GL_FLOAT,
                                  GL_FALSE, 0, static_cast<GLvoid*>(0));

            glBindTexture(GL_TEXTURE_2D, groundTexture);

            glUniform1i(groundTextureID, 0);

            glEnableVertexAttribArray(groundVertexUvID);
            glBindBuffer(GL_ARRAY_BUFFER, groundUvBuffer);
            glVertexAttribPointer(groundVertexUvID, 2, GL_FLOAT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));

            glDrawArrays(GL_QUADS, 0, 4);

            glDisableVertexAttribArray(groundVertexPositionModelspaceID);
            glDisableVertexAttribArray(groundVertexUvID);
        }

        glm::mat4 wallModelMatrix(1.f);

        glUniformMatrix4fv(wallModelMatrixID, 1, GL_FALSE, &wallModelMatrix[0][0]);

        for(int i : wallVertexBuffers)
        {
            glEnableVertexAttribArray(wallVertexPositionID);
            glBindBuffer(GL_ARRAY_BUFFER, i);
            glVertexAttribPointer(wallVertexPositionID, 3, GL_FLOAT, GL_FALSE, 0, static_cast<GLvoid*>(0));

            glBindTexture(GL_TEXTURE_2D, wallTexture);
            glUniform1i(wallTextureID, 0);

            glEnableVertexAttribArray(wallVertexUvID);
            glBindBuffer(GL_ARRAY_BUFFER, wallUvBuffer);
            glVertexAttribPointer(wallVertexUvID, 2, GL_FLOAT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));

            glDrawArrays(GL_QUADS, 0, 4);

            glDisableVertexAttribArray(wallVertexPositionID);
            glDisableVertexAttribArray(wallVertexUvID);
        }

        // Draw enemies

        for(int i = 0; i < numEnemies; i++)
        {
            glm::vec3 enemyPos(
                enemyBodyItrs[i]->body.getCenterOfMassPosition().x(),
                enemyBodyItrs[i]->body.getCenterOfMassPosition().y(),
                enemyBodyItrs[i]->body.getCenterOfMassPosition().z());
            glm::mat4 enemyModelMatrix =
                glm::translate(enemyPos) *
                glm::rotate(atan2(enemyPos.x, enemyPos.y), glm::vec3(0, 1, 0)) *
                glm::scale(glm::vec3(0.05f, 0.05f, 0.05f));

            glUniformMatrix4fv(enemyModelMatrixID, 1, GL_FALSE,
                               &enemyModelMatrix[0][0]);

            glEnableVertexAttribArray(enemyVertexPositionID);
            glBindBuffer(GL_ARRAY_BUFFER, enemyVertexBuffer[i]);
            glVertexAttribPointer(enemyVertexPositionID, 3, GL_FLOAT, GL_FALSE,
                                  0, static_cast<GLvoid*>(0));

            glBindTexture(GL_TEXTURE_2D, enemyTexture);

            glUniform1i(enemyTextureID, 0);

            glEnableVertexAttribArray(enemyVertexUvID);
            glBindBuffer(GL_ARRAY_BUFFER, enemyUvBuffer);
            glVertexAttribPointer(enemyVertexUvID, 2, GL_FLOAT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));

            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, enemyElementBuffer);
            glDrawElements(enemyObj.getPolygonType(),
                           enemyObj.getIndices().size(), GL_UNSIGNED_SHORT,
                           nullptr);

            glDisableVertexAttribArray(enemyVertexPositionID);
            glDisableVertexAttribArray(enemyVertexUvID);
        }
        // Draw ball
        glm::vec3 ballPos(ballItr->body.getCenterOfMassPosition().x(),
                          ballItr->body.getCenterOfMassPosition().y(),
                          ballItr->body.getCenterOfMassPosition().z());
        float ballAngle = ballItr->body.getOrientation().getAngle();
        glm::vec3 ballAxis(ballItr->body.getOrientation().getAxis().x(),
                           ballItr->body.getOrientation().getAxis().y(),
                           ballItr->body.getOrientation().getAxis().z());
        glm::mat4 ballTranslationMatrix = glm::translate(ballPos);
        glm::mat4 ballRotationMatrix = glm::rotate(ballAngle, ballAxis);
        glm::mat4 ballModelMatrix = ballTranslationMatrix * ballRotationMatrix *
                                    glm::scale(glm::vec3(10, 10, 10));

        glUniformMatrix4fv(ballModelMatrixID, 1, GL_FALSE,
                           &ballModelMatrix[0][0]);

        glBindTexture(GL_TEXTURE_2D, ballTexture);

        glUniform1i(ballTextureID, 0);

        glEnableVertexAttribArray(ballVertexPositionID);
        glBindBuffer(GL_ARRAY_BUFFER, ballVertexBuffer);
        glVertexAttribPointer(ballVertexPositionID, 3, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(ballVertexUvID);
        glBindBuffer(GL_ARRAY_BUFFER, ballUvBuffer);
        glVertexAttribPointer(ballVertexUvID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ballElementBuffer);
        glDrawElements(ballObj.getPolygonType(), ballObj.getIndices().size(),
                       GL_UNSIGNED_SHORT, nullptr);

        glDisableVertexAttribArray(ballVertexPositionID);
        glDisableVertexAttribArray(ballVertexUvID);

        // Draw gun
        glUniformMatrix4fv(m16ProjectionMatrixID, 1, GL_FALSE,
                           &m16ProjectionMatrix[0][0]);
        m16ViewMatrix = gunViewMatrixHandler(window);
        glUniformMatrix4fv(m16ViewMatrixID, 1, GL_FALSE, &m16ViewMatrix[0][0]);
        glUniformMatrix4fv(m16ModelMatrixID, 1, GL_FALSE,
                           &m16ModelMatrix[0][0]);

        glBindTexture(GL_TEXTURE_2D, m16Texture);

        glUniform1i(m16TextureID, 0);

        glEnableVertexAttribArray(m16VertexPositionModelspaceID);
        glBindBuffer(GL_ARRAY_BUFFER, m16VertexBuffer);
        glVertexAttribPointer(m16VertexPositionModelspaceID, 3, GL_FLOAT,
                              GL_FALSE, 0, static_cast<GLvoid*>(0));

        glEnableVertexAttribArray(m16VertexUvID);
        glBindBuffer(GL_ARRAY_BUFFER, m16UvBuffer);
        glVertexAttribPointer(m16VertexUvID, 2, GL_FLOAT, GL_FALSE, 0,
                              static_cast<GLvoid*>(0));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m16ElementBuffer);
        glDrawElements(m16Obj.getPolygonType(), m16Obj.getIndices().size(),
                       GL_UNSIGNED_SHORT, nullptr);

        glDisableVertexAttribArray(m16VertexPositionModelspaceID);
        glDisableVertexAttribArray(m16VertexUvID);

        glDisable(GL_DEPTH_TEST);

        // Draw 2D here
        // Draw time counter
        timeLimit = converter.from_bytes(
            std::to_string(TIME_LIMIT -
                           static_cast<int>(glfwGetTime() - timeBeforeLoop))
                .data());
        timeText.setText(timeLimit);
        timeText.render(1440 - textSize, 832 * 2 - textSize * 2);

        // Draw aim
        if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT))
        {
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glEnable(GL_BLEND);
            glEnableVertexAttribArray(aimVertexPositionID);
            glBindBuffer(GL_ARRAY_BUFFER, aimVertexBuffer);
            glVertexAttribPointer(aimVertexPositionID, 2, GL_FLOAT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));

            glBindTexture(GL_TEXTURE_2D, aimTexture);

            glUniform1i(aimTextureID, 0);

            glEnableVertexAttribArray(aimVertexUvID);
            glBindBuffer(GL_ARRAY_BUFFER, aimUvBuffer);
            glVertexAttribPointer(aimVertexUvID, 2, GL_INT, GL_FALSE, 0,
                                  static_cast<GLvoid*>(0));

            glDrawArrays(GL_QUADS, 0, 4);

            glDisableVertexAttribArray(aimVertexPositionID);
            glDisableVertexAttribArray(aimVertexUvID);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();

    } while(glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS &&
            glfwWindowShouldClose(window) == 0);

    glDeleteBuffers(1, &m16VertexBuffer);
    glDeleteBuffers(1, &m16UvBuffer);
    glDeleteBuffers(1, &m16ElementBuffer);
    glDeleteBuffers(splitNumber * splitNumber, groundVertexBuffer);
    glDeleteBuffers(1, &groundUvBuffer);
    glDeleteBuffers(4, wallVertexBuffers);
    glDeleteBuffers(1, &wallUvBuffer);
    glDeleteBuffers(1, &ballVertexBuffer);
    glDeleteBuffers(1, &ballUvBuffer);
    glDeleteBuffers(1, &ballNormalBuffer);
    glDeleteBuffers(1, &ballElementBuffer);
    glDeleteBuffers(numEnemies, enemyVertexBuffer);
    glDeleteBuffers(1, &enemyUvBuffer);
    glDeleteBuffers(1, &enemyElementBuffer);
    glDeleteBuffers(1, &aimVertexBuffer);
    glDeleteBuffers(1, &aimUvBuffer);
    glDeleteProgram(worldProgramID);
    glDeleteProgram(aimProgramID);
    glDeleteTextures(1, &m16Texture);
    glDeleteTextures(1, &groundTexture);
    glDeleteTextures(1, &wallTexture);
    glDeleteTextures(1, &ballTexture);
    glDeleteTextures(1, &enemyTexture);
    glDeleteTextures(1, &aimTexture);

    return true;
}

double getDeltaTime(double* lastTime)
{
    double currentTime = glfwGetTime();
    double deltaTime = currentTime - *lastTime;
    *lastTime = currentTime;
    return deltaTime;
}

glm::mat4 gunViewMatrixHandler(GLFWwindow* window)
{
    glfwSetInputMode(window, GLFW_STICKY_MOUSE_BUTTONS, GL_TRUE);
    glm::mat4 gunViewMatrix;
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
        gunViewMatrix =
            glm::lookAt(glm::vec3(3.2f, 1.5f, 0), glm::vec3(-10, 1.5f, 0),
                        glm::vec3(0, 1, 0));
    }
    else
    {
        gunViewMatrix = glm::lookAt(
            glm::vec3(3, 1, 1), glm::vec3(-100000, 0, 0), glm::vec3(0, 1, 0));
    }
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
        fire();
    }
    return gunViewMatrix;
}

void fire()
{
}
