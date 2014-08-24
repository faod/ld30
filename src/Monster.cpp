#include "Monster.hpp"
#include "Game.hpp"
#include "failure.hpp"

spAtlas* Monster::modelAtlas = NULL;
spSkeletonJson* Monster::jsonSkel = NULL;
spAnimationStateData* Monster::stateData = NULL;

Monster::Monster(Game& g, b2Vec2 p) : Character(g), swordFix(NULL), life(100)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.fixedRotation = true;

	body = g.w.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(.25f, 0.9f);

	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 5.0f;
	fixtureDef.friction = 0.9f;
	fixtureDef.filter.categoryBits = MONSTER;
	fixtureDef.filter.maskBits = PLAYER | WALL | MONSTER | FOOT;

	body->CreateFixture(&fixtureDef);

	//sword to kill player
	dynamicBox.SetAsBox(0.4, 0.25, b2Vec2(0.65, -0.2), 0);
	swordDef.shape = &dynamicBox;
	swordDef.isSensor = true;
	swordDef.density = 0.1;
	swordDef.filter.categoryBits = SWORD;
	swordDef.filter.maskBits = PLAYER;
	
	swordFix = body->CreateFixture(&swordDef);
	swordFix->SetUserData(this);

	// Loads the Spine model
	ALLEGRO_PATH *path, *resourceDir, *file;
	resourceDir= al_get_standard_path(ALLEGRO_RESOURCES_PATH);
	std::cerr << al_path_cstr(resourceDir, '/') << std::endl;

	if (modelAtlas == NULL) {
		file = al_create_path("data/animations/skeleton.atlas");
		path =  al_clone_path(resourceDir);
		al_join_paths(path, file); al_destroy_path(file);
		modelAtlas = spAtlas_createFromFile(al_path_cstr(path, '/'), NULL);
		if (!modelAtlas) throw Failure("Failed to load the monster's atlas.");
		al_destroy_path(path);
		jsonSkel = spSkeletonJson_create(modelAtlas);
	}

	file = al_create_path("data/animations/skeleton.json");
	path =  al_clone_path(resourceDir);
	al_join_paths(path, file); al_destroy_path(file);
	modelData = spSkeletonJson_readSkeletonDataFile(jsonSkel, al_path_cstr(path, '/'));
	if (!modelData) throw Failure("Failed to load the monster's data.");
	al_destroy_path(path); al_destroy_path(resourceDir);

	if (stateData == NULL) {
		stateData = spAnimationStateData_create(modelData);
		spAnimationStateData_setMixByName(stateData, "walk", "rest", 0.2f);
		spAnimationStateData_setMixByName(stateData, "rest", "walk", 0.2f);
		spAnimationStateData_setMixByName(stateData, "rest", "slash", 0.1f);
		spAnimationStateData_setMixByName(stateData, "slash", "rest", 0.1f);
		spAnimationStateData_setMixByName(stateData, "walk", "slash", 0.1f);
		spAnimationStateData_setMixByName(stateData, "slash", "walk", 0.1f);
	}

	model = loadSkeleton(modelData, stateData);
	if (!model) throw Failure("Failed to load the monster's skeleton.");

	spAnimationState_setAnimationByName(model->state, 0, "slash",  true);
}

Monster::~Monster()
{
	spSkeletonData_dispose(modelData);
	disposeSkeleton(model);
}

void Monster::tick()
{
	b2Vec2 screen = g.getScreenCorner();
	model->skeleton->x = (body->GetPosition().x) * g.pixelpm - screen.x;
	model->skeleton->y = (body->GetPosition().y + .9)  * g.pixelpm - screen.y;
	skeletonUpdate(model, 1 / (float) g.m.animation_tick);
}

void Monster::draw() const
{
	skeletonDraw(model);
	
	//hp bar
	const b2Vec2 screen = g.getScreenCorner();
	const b2Vec2 pos 	= body->GetPosition();

	al_draw_rectangle((pos.x -0.25f) * Game::pixelpm - screen.x,
					  (pos.y -0.9f)  * Game::pixelpm - screen.y - 15,
					  (pos.x + 0.25f) * Game::pixelpm - screen.x,
					  (pos.y -0.9f)  * Game::pixelpm - screen.y -10,
					  al_map_rgb(80, 0, 0),
					  1);
	al_draw_filled_rectangle((pos.x -0.25f) * Game::pixelpm - screen.x + 1,
							 (pos.y -0.9f ) * Game::pixelpm - screen.y - 14,
							 (pos.x -0.25f) * Game::pixelpm - screen.x + (life * (0.5f * Game::pixelpm - 2)) / 100,
							 (pos.y -0.9f ) * Game::pixelpm - screen.y - 11,
							 al_map_rgb(255, 0, 0));
	
}

void Monster::moveLeft()
{
	//animation left
	model->skeleton->flipX = 1;
	std::cout << "Monster moved on the left" << std::endl;

	//sword hitbox left
	body->DestroyFixture(swordFix);
	dynamicBox.SetAsBox(0.4, 0.25, b2Vec2(-0.65, -0.2), 0);
	swordDef.shape = &dynamicBox;
	swordFix = body->CreateFixture(&swordDef);
	swordFix->SetUserData(this);
}

void Monster::moveRight()
{
	//animation right
	model->skeleton->flipX = 0;
	std::cout << "Monster moved on the right" << std::endl;

	//sword hitbox right
	body->DestroyFixture(swordFix);
	dynamicBox.SetAsBox(0.4, 0.25, b2Vec2(0.65, -0.2), 0);
	swordDef.shape = &dynamicBox;
	swordFix = body->CreateFixture(&swordDef);
	swordFix->SetUserData(this);
}

void Monster::jump()
{
	std::cout << "Monster jumped Cyto's mommy" << std::endl;
}
