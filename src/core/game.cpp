#include "game.h"
#include "mesh.h"
#include "engine_consts.h"


namespace game {


	
	Game::Game(std::shared_ptr<GameParameters> params)
		:params(params), arrowSprite(params->arrowTexture),
		 circleSprite(params->circleTexture), targetSprite(params->targetTexture)
		{
			targetCoords = funcs::genRandomCoords2d(params->minRange, params->maxRange);

			params->airplane.bindOnDrop(
				std::bind(&Game::onDrop, this)
			);

			/*maybe i should define these in the params too*/
			// glm::mat4 local(1.0f);
			// local = glm::translate(local, glm::vec3(30.0f/2, 30.0f/2, 0.0f));
			// local = glm::rotate(local, glm::radians(180.0f), Z_AXIS);
			// local = glm::translate(local, glm::vec3(-30.0f/2, -30.0f/2, 0.0f));
			// local = glm::scale(local, glm::vec3(30.0f));
			// arrowSprite.setLocalTransform(local);
			arrowSprite.setSize(30.0f);
			//should be set in the way s.t the arrow is pointing to the right
			arrowSprite.setRotation(glm::radians(180.0f));

			// local = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f));
			// circleSprite.setLocalTransform(local);
			circleSprite.setSize(100.0f);

			targetSprite.setSize(10.0f);
			targetSprite.setPulsating(true);
		}

	void Game::update(float dt){
		params->terrainSystem.update(dt);

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

	void Game::draw(Shader &textShader, Shader &screenShader, Shader &spriteShader)
	{        

		params->terrainSystem.draw(fbo);

		// fbo.BindWithoutClear();
		// spriteShader.use();

		glm::vec2 airplanePos = glm::vec2(params->airplane.getPos().x, 
										  params->airplane.getPos().z);

		glm::vec2 diff = (targetCoords - airplanePos);
		float dist     = glm::length(diff);
		glm::vec2 dir  = diff / dist;
		
		// glm::vec3 localVel = params->airplane.getLocalLinVel();
		glm::vec3 currentDir = params->airplane.getDirection();
		float currentTheta = std::atan2(currentDir.z, currentDir.x);
		// float theta = std::atan2(-dir.y, dir.x);

		// std::cout << glm::degrees(currentTheta) 
		//     << std::endl;

		// // glm::mat4 view = glm::translate(glm::mat4(1.0f), 
		//                     // glm::vec3(0.0f, 0.3f, -1.0f));
		// glm::mat4 view(1.0f);
		// spriteShader.setMatrix("view", view );
		// // spriteShader.setMatrix("view", glm::mat4(1.0f));
		// // // glm::vec3 pos = params->camera.position + params->camera.direction*3.0f 
		// //                 // + params->camera.getUp();
		// glm::vec3 pos = glm::vec3(0.0f, -0.3f, -1.0f);
		// glm::mat4 model(1.0f);
		// model = glm::translate(model, pos);
		// model = glm::rotate(model, theta, glm::vec3(0.0f, 1.0f, 0.0f));
		// spriteShader.setMatrix("model", model);
		// arrowSprite.draw(spriteShader);

		// pos = glm::vec3(0.0f, 0.3f, -1.0f);
		// model = glm::translate(glm::mat4(1.0), pos);
		// model = glm::rotate(model, currentTheta, Y_AXIS);
		// spriteShader.setMatrix("model", model);
		// arrowSprite.draw(spriteShader);
		// fbo.unBind();

		params->cloudSystem.update(fbo);
		params->cloudSystem.draw(screenShader);

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
				/*text*/
				"DROP HERE"
				, 
				/*x, y TO DO: use global params*/
				400.0f, 20.0f,
				/* scale*/
			funcs::remap(std::sin(
				glfwGetTime()*2.0f
			), -1., 1., 0.9, 1.)

			, params->coordsTextColor, 
			TextRenderer::CENTER
			);

			// if (params->airplane.dropped())
			//     targetCoords = funcs::genRandomCoords2d();
		}
		// else
		//     params->textRenderer.renderText(textShader, 
		//         T.str()
		//         , 0.0f, 20.0f,
		//     1.0f
		//     , params->coordsTextColor, 
		//     TextRenderer::TOP_LEFT
		//     );
			

		// std::ostringstream T2;
		// T2 << '(' << targetCoords.x << ',' << targetCoords.y 
		//   << ')';
		// params->textRenderer.renderText(textShader, 
		// 	T2.str(),
		// 	0.0f, 600.0f,
		// 	1.0f, params->tCoordsTextColor, 
		// 	TextRenderer::BOTTOM_LEFT
		// );



		spriteShader.use();
		glm::vec3 pos = glm::vec3(700.0f, 500.0f, 0.0f);
		params->spriteRenderer.draw(circleSprite, spriteShader, pos);

		pos = glm::vec3(735.0f, 535.0f, 0.0f); 
		params->spriteRenderer.draw(arrowSprite, spriteShader, pos, currentTheta);

		pos = glm::vec3(glm::vec2(750.0f, 550.0f) + dir * std::min(dist/10.0f, 45.0f),
		 0.0f);
		params->spriteRenderer.draw(targetSprite, spriteShader, pos, 0.0f, 
									SpriteRenderer::CENTER);

	}

	void Game::onDrop(){
		this->dropped = true;
	}

	void Game::addConfigParamsToImgui(){
		// ImGui::ColorEdit3("Text Color", &params->coordsTextColor[0]);
		// ImGui::ColorEdit3("Target Text Color", &params->tCoordsTextColor[0]);
	}

	Game::~Game(){
	}

}