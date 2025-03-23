#include "sprite_renderer.h"
#include "engine_consts.h"

Sprite::Sprite(std::string fname, float size, float rotation, bool pulsating)
    :size(size), rotation(rotation), pulsating(pulsating)
    {
        id = funcs::TextureFromFile(fname);
    }

void Sprite::setSize(float size)
    {
        this->size = size;
    }
    
float Sprite::getSize(){
    return size;
}

void Sprite::setPulsating(bool flag){
    this->pulsating = flag;
}

void Sprite::setRotation(float rot)
    {
        rotation = rot;
    }

glm::mat4 Sprite::getRotationMatrix(float theta) //around center
    {
        glm::mat4 matrix(1.0f);
        matrix = glm::translate(matrix, glm::vec3(size/2, size/2, 0.0f));
        matrix = glm::rotate(matrix, theta, Z_AXIS);
        matrix = glm::translate(matrix, glm::vec3(-size/2, -size/2, 0.0f));
        return matrix;
    }

void Sprite::bind(Shader& shader){
        if (id){
            bool blendEnabled = glIsEnabled(GL_BLEND);

            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            shader.use();
            glm::mat4 local = getRotationMatrix(this->rotation);
            float sz = size;
            if (pulsating)
                sz = funcs::remap(std::sin((float)glfwGetTime()*3.0f), -1.0f, 1.0f, 
                                  0.7f, 1.0f)*size; 
            local = glm::scale((local), glm::vec3(sz, sz, 0.0f));
            shader.setMatrix("local", local);
            shader.setInt("texture_diffuse1", 0);
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, id);
            // plane.draw(shader, id);

            if (!blendEnabled)
                glDisable(GL_BLEND);
        }
    }

Sprite::~Sprite()
    {
        glDeleteTextures(1, &id);
    }

SpriteRenderer::SpriteRenderer(){
        initVertexData();
    }

void SpriteRenderer::initVertexData(){
        if (!ready){
            float verts[] = {
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f,
                0.0f, 0.0f, 0.0f, 0.0f,
        
                0.0f, 1.0f, 0.0f, 1.0f,
                1.0f, 1.0f, 1.0f, 1.0f,
                1.0f, 0.0f, 1.0f, 0.0f
            };

            glGenVertexArrays(1, &VAO);
            glGenBuffers(1, &VBO);

            glBindVertexArray(VAO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)(2*sizeof(float)));

            glBindVertexArray(0);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
            ready = true;
        }
    }

void SpriteRenderer::draw(Sprite& sprite, Shader& shader, glm::vec3 pos, 
                          float rotation, Mode mode)
{
  if (ready)
  {
      // std::cout << "DRAWING" << std::endl;
      glBindVertexArray(VAO);
      sprite.bind(shader);
      shader.use();
      switch (mode)
      {
        case TOP_LEFT:
            break;
        case CENTER:
            pos -= glm::vec3(glm::vec2(sprite.getSize())/2.0f, 0.0f);
            break;
      }
      glm::mat4 model = glm::translate(glm::mat4(1.0f), pos);
      if (rotation != 0.0f)
      {
          model *= sprite.getRotationMatrix(rotation);
      }
      shader.setMatrix("model", model);
      glDrawArrays(GL_TRIANGLES, 0, 6);
      glBindVertexArray(0);
  } else {
      // std::cout << "NOT READY " << std::endl;
  }
}

SpriteRenderer::~SpriteRenderer()
{
    // glDeleteVertexArrays(1, &VAO);
}