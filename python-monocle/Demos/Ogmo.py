from Monocle import *
from random import random
Monocle = Monocle()

FRICTION_GROUND = 800
FRICTION_AIR = 400
GRAVITY = 300
JUMP = 6000
MAXSPEED_GROUND = 60.0
MAXSPEED_AIR = 100.0
ACCELERATION = 800
WALLJUMP = 8000

class Player(Entity):
    def __init__(self,x,y):
        Entity.__init__(self)
        self.position = Vector2(x,y)
        self.doubleJump = False
        self.cling = 0
        self.clingDir = 0
        self.onGround = False
        self.direction = True
        self.tag = "PLAYER"
        self.AddTag(self.tag)
        self.SetCollider(RectangleCollider(8,8))
        self.sprite = SpriteAnimation("player.png",FILTER_NONE,8,8)
        self.sprite.Add("stand",0,0,0)
        self.sprite.Add("run",0,3,12.0)
        self.sprite.Add("jumpUp",8,8,0)
        self.sprite.Add("jumpDown",9,9,0)
        self.sprite.Play("run")
        self.SetLayer(-1)
        self.SetGraphic(self.sprite)
    def Update(self):
        if(Input.IsKeyMaskHeld("left")):
            if(self.cling < 0):
                self.velocity.x -= ACCELERATION * Monocle.deltaTime
            self.sprite.Play("run")
            self.direction = False
            self.scale.x = -1
        elif (Input.IsKeyMaskHeld("right")):
            if(self.cling < 0):
                self.velocity.x += ACCELERATION * Monocle.deltaTime
            self.sprite.Play("run")
            self.direction = True
            self.scale.x = 1
        else:
            self.sprite.Play("stand")
            friction
            if(self.onGround):
                self.velocity.x = APPROACH(self.velocity.x, 0, FRICTION_GROUND * Monocle.deltaTime)
            else:
                self.velocity.x = APPROACH(self.velocity.x, 0, FRICTION_AIR * Monocle.deltaTime)
        if(Input.IsKeyMaskPressed("jump") and (self.onGround or self.cling > 0 or not self.doubleJump)):
            #jump
            self.velocity.y = -JUMP * Monocle.deltaTime
            if(not self.onGround):
                if(self.cling > 0):
                    self.velocity.x = self.clingDir * WALLJUMP * Monocle.deltatime
                    self.cling = -1
                if(self.cling < 0):
                    self.doubleJump = True
        #gravity
        self.velocity.y += GRAVITY * Monocle.deltaTime
        #max speed
        maxSpeed = MAXSPEED_GROUND if self.onGround else MAXSPEED_AIR
        if(abs(self.velocity.y) > maxSpeed):
            self.velocity.x = SIGN(self.velocity.x, 1) * maxSpeed
        #Motion
        self.velocity.x,self.position.x = self.Motion(self.velocity.x,self.position.x)
        self.velocity.y,self.position.y = self.Motion(self.velocity.y,self.position.y)
        #Check for ground
        if(self.CollideAt("WALL",self.position.x, self.position.y + 1)):
           self.onGround = True
           self.doubleJump = False
        else:
           self.onGround = False
           #change sprite
           if(self.velocity.y < 0):
               self.sprite.Play("jumpUp")
           else:
               self.sprite.Play("jumpDown")
           #check for wall jump
           if(self.CollideAt("WALL", self.position.x + 1, self.position.y) and Input.IsKeyMaskHeld("right")):
               self.cling = 10
               self.clingDir = -1
           elif(self.CollideAt("WALL", self.position.x - 1, self.position.y) and Input.IsKeyMaskHeld("left")):
               self.cling = 10
               self.clingDir = 1
           #decrease how long we can cling to a wall for
           self.cling -= 1
           #and at the end of the day, are we dead?
           if(self.Collide("SPIKE")):
               World.ResetCoins()
    def Motion(self,speed,to):
           #move
           to += speed * Monocle.deltaTime
           #collide
           collide = False
           while(self.Collide("WALL")):
               to -= SIGN(speed,0.1)
               collide = True
           #stop motion on Collision
           if(collide):
               speed = 0
           return speed,to
    def CollideAt(self, tag, x, y):
           pre = Vector2(self.position.x,self.position.y)
           collide = False
           self.position.x = x
           self.position.y = y
           if(self.Collide(tag)):
               collide = True
           self.position = Vector2(pre.x,pre.y)
           return collide
class Wall(Entity):
    def __init__(self,x,y):
           Entity.__init__(self)
           self.position = Vector2(x,y)
           self.AddTag("WALL")
           self.SetCollider(RectangleCollider(8,8))
           self.SetLayer(5)
    def Render(self):
           Graphics.PushMatrix()
           Graphics.Translate(self.position)
           Graphics.BindTexture(None)
           Graphics.RenderQuad(8, 8)
           Graphics.PopMatrix()
class Coin(Entity):
    def __init__(self,x,y,sprite):
           Entity.__init__(self)
           self.position = Vector2(x,y)
           self.collected = False
           self.start = Vector2(x,y)
           self.reset = False
           self.SetLayer(-1)
           self.SetGraphic(sprite)
           self.AddTag("COIN")
           self.SetCollider(RectangleCollider(8,8))
           #while(self.Collide("WALL") is not None):
           #    self.position = Vector2((random() % 160/8) * 8 + 4, (random() % 120 / 8) * 8 + 4)
           #    start = Vector2(self.position.x,self.position.y)
    def Update(self):
           if(self.Collide("PLAYER") and not reset):
               self.collected = True
           if(self.collected and self.isVisible):
               #scale out
               self.scale.x -= 0.1
               self.scale.y -= 0.1
               #follow player
               self.position.x -= (self.position.x - World.player.position.x) / 10
               self.position.y -= (self.position.y - World.player.position.y) / 10
               if(self.scale.x < 0):
                   self.isVisible = False
           if(self.reset):
               self.isVisible = True
               #scale in
               if(self.scale.x is not 1):
                   self.scale.x += 0.1
                   self.scale.y += 0.1
               #slide to start position
               self.position.x -= (self.position.x - self.start.x) / 10
               self.position.y -= (self.position.y - self.start.y) / 10
               if(abs(self.position.x - self.start.x) < 0.2 and abs(self.position.y - self.start.y) < 0.2):
                   self.reset = False
                   self.position.x = self.start.x
                   self.position.y = self.start.y
class Spike(Entity):
    def __init__(self,x,y,sprite):
        Entity.__init__(self)
        self.position = Vector2(x,y)
        self.SetLayer(-1)
        self.SetGraphic(sprite)
        self.AddTag("SPIKE")
        self.SetCollider(RectangleCollider(8,8))
        if(self.position.x is -1 and self.position.y is -1):
           self.position = Vector2((random() % 160/8) * 8 + 4, (random() % 120 / 8) * 8 + 4)
        #   while(self.Collide("WALL")):
        #       self.position = Vector2((random() % 160/8) * 8 + 4, (random() % 120 / 8) * 8 + 4)
        #   while(not self.Collide("WALL")):
        #       self.position.y += 1
class World(Scene):
    def __init__(self):
        Scene.__init__(self)
        self.instance = self
        self.player = None
        self.wall = None
        self.spike = None
        self.atCoin = None
        self.atSpike = None
        self.coins = []
    def Begin(self):
        Scene.Begin(self)
        Graphics.Set2D(160,120)
        Graphics.SetCameraPosition(Vector2(80,60))
        Assets.SetContentPath("../../Content/Ogmo/")
        self.atCoin = Sprite("coin.png",FILTER_NONE,8,8)
        self.atSpike = Sprite("spike.png",FILTER_NONE,8,8)
        Input.DefineMaskKey("jump",KEY_UP)
        Input.DefineMaskKey("jump",KEY_Z)
        Input.DefineMaskKey("jump",KEY_A)
        Input.DefineMaskKey("left",KEY_LEFT)
        Input.DefineMaskKey("right",KEY_RIGHT)
        Input.DefineMaskKey("kill",KEY_K)
        Level.SetScene(self)
        Level.LoadProject("project.xml")
        Level.Load("level01.xml")
        self.player = Player(120,8)
        self.Add(self.player)
        for i in range(160):
            self.wall = Wall(i+4,116)
            self.Add(self.wall)
            i+=7
        for i in range(120):
            self.wall = Wall(4, i + 4)
            self.Add(self.wall)
            i += 7
        for i in range(80):
            self.wall = Wall(60, i + 4)
            self.Add(self.wall)
            i+=7
        for i in range(18):
            self.wall = Wall(12 + i, 28)
            self.Add(self.wall)
            i += 8
        for i in range(10):
            coin = Coin((random() % 160/8) * 8 + 4,(random() % 120/8) * 8 + 4,self.atCoin)
            self.coins.append(coin)
            self.Add(coin)
        for i in range(8):
            self.Add(Spike(-1,-1,self.atSpike))
    def Update(self):
        Scene.Update(self)
        if(Input.IsKeyPressed(KEY_S) and Input.IsKeyHeld(KEY_LCTRL)):
            print "saving level..."
            Level.Save()
    def ResetCoins(self):
        for coin in self.coins:
            if(coin.collected):
                coin.collected = False
                coin.position = Vector2(self.player.position.x,self.player.position.y)
                coin.reset = True
    def End(self):
        print "bye!"

game = Game()
game.Init()
scene = World()
game.SetScene(scene)
game.Main()
