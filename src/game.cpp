#include "game.h"


namespace game {


    
    Game::Game(std::shared_ptr<GameParameters> params)
        :params(params) {
            targetCoords = funcs::genRandomCoords2d(params->minRange, params->maxRange);

            params->airplane.bindOnDrop(
                std::bind(&Game::onDrop, this)
            );
        }

    void Game::update(float dt){
        glm::vec2 p = 
        {params->airplane.getPos().x,
         params->airplane.getPos().z};
        if (glm::length2(p - targetCoords) < params->distThresholdSqr && 
            this->dropped)
        {
            targetCoords = funcs::genRandomCoords2d(params->minRange, params->maxRange);
        }

        this->dropped = false;
    }

    void Game::draw(Shader& textShader)
    {

		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glm::vec2 p = {params->airplane.getPos().x,
                       params->airplane.getPos().z};
		std::ostringstream T;

		T.precision(4);
		T << '(' << p.x << ',' << p.y
		 << ')';
        
        // std::cout << glm::length2(p - targetCoords) << std::endl;
        if (glm::length2(p - targetCoords) < params->distThresholdSqr ){
            params->textRenderer.renderText(textShader, 
                "DROP HERE"
                , 400.0f, 20.0f,
            funcs::remap(std::sin(
                glfwGetTime()*2.0f
            ), -1., 1., 0.9, 1.)
            , params->coordsTextColor, 
            TextRenderer::CENTER
            );

            // if (params->airplane.dropped())
            //     targetCoords = funcs::genRandomCoords2d();
        }
        else
            params->textRenderer.renderText(textShader, 
                T.str()
                , 0.0f, 20.0f,
            1.0f
            , params->coordsTextColor, 
            TextRenderer::TOP_LEFT
            );
            

		std::ostringstream T2;
		T2 << '(' << targetCoords.x << ',' << targetCoords.y 
		  << ')';
		params->textRenderer.renderText(textShader, 
			T2.str(),
			0.0f, 600.0f,
			1.0f, params->tCoordsTextColor, 
			TextRenderer::BOTTOM_LEFT
		);

    }

    void Game::onDrop(){
        this->dropped = true;
    }

    void Game::addConfigParamsToImgui(){
        ImGui::ColorEdit3("Text Color", &params->coordsTextColor[0]);
        ImGui::ColorEdit3("Target Text Color", &params->tCoordsTextColor[0]);
    }

}