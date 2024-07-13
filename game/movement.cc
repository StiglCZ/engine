#include "../types.hh"
#include <unistd.h>
#define SPEED 0.006
extern "C"{
    GameData* d;
    u8 W, S, A, D,
       space, esc,
       T, F, G, H;
    fx f = 0, wp = 10, ap = 0;
    void Init(GameData* gd, u32 scriptIndex){
        GameObject go = emptyGameObj;
        go.script = scriptIndex;
        d = gd;
        d->gameObjects->push_back(go);
        d->cp->pos = {0, 0, 0};
    }
    void Start(u32 index){
        (void)index; // Unused
        W = getKey(d, "w");
        A = getKey(d, "a");
        S = getKey(d, "s");
        D = getKey(d, "d");
        space = getKey(d, "space");
        T = getKey(d, "t");
        F = getKey(d, "f");
        G = getKey(d, "g");
        H = getKey(d, "h");
        esc = getKey(d, "Escape");
    }
    void Update(u32 index){
        (void)index; // Unused
        if(d->keys[space]){
            f += 0.01;
            d->cp->rot = {0, f, 0};
            d->cp->sync(d->cp, false);
        }
        float speed = SPEED * getDelta(d);
        if(d->keys[W]) d->cp->pos += d->cp->rot.Forward() * speed;
        if(d->keys[S]) d->cp->pos -= d->cp->rot.Forward() * speed;
        if(d->keys[A]) d->cp->pos += d->cp->rot.Right() * speed;
        if(d->keys[D]) d->cp->pos -= d->cp->rot.Right() * speed;
        
        // Moving the physics object
        speed /= 2;
        if(d->keys[T]) ((*d->gameObjects)[3]).position.Z += speed;
        if(d->keys[G]) ((*d->gameObjects)[3]).position.Z -= speed;
        if(d->keys[F]) ((*d->gameObjects)[3]).position.X -= speed;
        if(d->keys[H]) ((*d->gameObjects)[3]).position.X += speed;

        // Pause
        if(d->keys[esc]){
            d->drawText({(int)d->cp->w / 2, (int)d->cp->h / 2}, "PAUSED");
            while(+d->keys[esc]) usleep(1);
            while(!d->keys[esc]) usleep(1);
            while(+d->keys[esc]) usleep(1);
        }
    }
}
