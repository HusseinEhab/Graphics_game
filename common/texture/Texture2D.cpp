//
// Created by ahmad on 31/12/2020.
//

#include "Texture2D.h"
Texture2D::Texture2D()
{
}

Texture2D::Texture2D(const char* file){
    glGenTextures(1, &this->texture);
    our::texture_utils::loadImage(this->texture, file);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, this->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

}
void Texture2D::ActivateTexture(const char * filename, bool generate_mipmap) {

            if (this->texture)
        {
            glDeleteTextures(1, &this->texture);
        }
        //Generate the Texture and bind them
        glGenTextures(1, &this->texture);
//        glBindTexture(GL_TEXTURE_2D, this->texture);
        //load new Texture
        our::texture_utils::loadImage(this->texture, filename);
        //Activate and bind the new texture
//        glActiveTexture(GL_TEXTURE0);
//        glBindTexture(GL_TEXTURE_2D, this->texture);
        // for enabling the texture
//        glEnable(GL_TEXTURE_2D);

}
void Texture2D::LinkTexture(int unit) {

    if(unit == 0){
        glActiveTexture(GL_TEXTURE0 );
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }
    if(unit == 1){
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }
    if(unit == 2){
        glActiveTexture(GL_TEXTURE2 );
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }
    if(unit == 3){
        glActiveTexture(GL_TEXTURE3 );
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }
    if(unit == 4){
        glActiveTexture(GL_TEXTURE4 );
        glBindTexture(GL_TEXTURE_2D, this->texture);
    }


}
