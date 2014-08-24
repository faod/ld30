#include "Monster.hpp"
#include "Game.hpp"
#include "failure.hpp"

spAtlas* Monster::modelAtlas = NULL;
spSkeletonJson* Monster::jsonSkel = NULL;
spAnimationStateData* Monster::stateData = NULL;

Monster::Monster(Game& g, b2Vec2 p) : Character(g)
{
	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(p.x, p.y);
	bodyDef.fixedRotation = true;

	body = g.w.CreateBody(&bodyDef);

	dynamicBox.SetAsBox(.25f, 0.9f);

	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.9f;
	fixtureDef.filter.categoryBits = MONSTER;
	fixtureDef.filter.maskBits = PLAYER | WALL | MONSTER;

	body->CreateFixture(&fixtureDef);

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

	spAnimationState_setAnimationByName(model->state, 0, "rest",  true);
}

Monster::~Monster()
{
	spSkeletonData_dispose(modelData);
	disposeSkeleton(model);
}

void Monster::tick()
{
	model->skeleton->x = (body->GetPosition().x + .25) * g.pixelpm;
	model->skeleton->y = (body->GetPosition().y + .9)  * g.pixelpm;
	skeletonUpdate(model, 1 / (float) g.m.animation_tick);
}

void Monster::draw() const
{
	//al_draw_bitmap(bm, body->GetPosition().x * Game::pixelpm, body->GetPosition().y * Game::pixelpm, 0);
	skeletonDraw(model);
}

void Monster::moveLeft()
{
	model->skeleton->flipX = 1;
	std::cout << "Monster moved on the left" << std::endl;
}

void Monster::moveRight()
{
	model->skeleton->flipX = 0;
	std::cout << "Monster moved on the right" << std::endl;
}

void Monster::jump()
{
	std::cout << "Monster jumped Cyto's mommy" << std::endl;
}
