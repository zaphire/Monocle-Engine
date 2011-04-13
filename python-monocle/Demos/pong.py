from Monocle import *
Monocle = Monocle()
class Paddle(Entity):
        def __init__(self):
                Entity.__init__(self)
                self.speed = 0.0
                self.accel = 2000.0
                self.maxSpeed = 300.0
                self.friction = 500.0
                self.maxY = 600.0
                self.keyUp = None
                self.keyDown = None
                self.AddTag("Paddle")
                rectangleCollider = RectangleCollider(25.0,100.0)
                self.SetCollider(rectangleCollider)
        def Update(self):
                Entity.Update(self)
                if(Input.IsKeyHeld(self.keyUp)):
                        self.speed += self.accel * Monocle.deltaTime
                        if(self.speed > self.maxSpeed):
                                self.speed = self.maxSpeed
                elif (Input.IsKeyHeld(self.keyDown)):
                        self.speed -= self.accel * Monocle.deltaTime
                        if(self.speed < -self.maxSpeed):
                                self.speed = -self.maxSpeed
                else:
                        if(self.speed > 0.0):
                                self.speed -= self.friction * Monocle.deltaTime
                        elif (self.speed < 0.0):
                                self.speed += self.friction * Monocle.deltaTime
                        if(abs(self.speed) < 0.05):
                                self.speed = 0.0
                self.position += Vector2.up * Monocle.deltaTime * self.speed
                if(self.position.y > self.maxY):
                        self.position.y = self.maxY
                        self.speed = -1.0
                elif(self.position.y < 0):
                        self.position.y = 0
                        self.speed = 1.0
        def Render(self):
                Entity.Render(self)
                Graphics.PushMatrix()
                Graphics.Translate(self.position)
                Graphics.RenderQuad(25.0,100.0)
                Graphics.PopMatrix();

class Ball(Entity):
        def __init__(self):
                Entity.__init__(self)
                self.texture = None
                self.AddTag("Ball")
                rectangleCollider = RectangleCollider(25.0,25.0)
                self.SetCollider(rectangleCollider)
                self.velocity = Vector2.right * 200.0
        def Update(self):
                Entity.Update(self)
                lastPosition = self.position
                self.position += self.velocity * Monocle.deltaTime
                collider = self.Collide("Paddle")
                if(collider):
                        entity = collider.GetEntity()
                        self.position = lastPosition
                        diff = self.position - collider.GetEntity().position
                        diff.Normalize()
                        diff *= self.velocity.GetMagnitude()
                        self.velocity = diff
                #if we hit the roof or floor, bounce!
                if(self.position.y < 0 or self.position.y > 600):
                        self.position = lastPosition
                        self.velocity.y *= -1
                #if we reach a player's goal, reset the ball!
                if(self.position.x < 0):
                        self.SendNoteToScene("BallOffLeft")
                if(self.position.x > 800):
                        self.SendNoteToScene("BallOffRight")
        def Render(self):
                Entity.Render(self)
                Graphics.BindTexture(self.texture)
                Graphics.PushMatrix();
                Graphics.Translate(self.position)
                Graphics.RenderQuad(25.0,25.0)
                Graphics.PopMatrix();

class GameScene(Scene):
        def __init__(self):
                Scene.__init__(self)
                self.ball = None
                self.paddle1 = None
                self.paddle2 = None
        def Begin(self):
                #Debug.Log("Pong::GameScene::Begin()!")
                print "Pong::GameScene::Begin()!"
                Scene.Begin(self)
                #do pong specific init
                self.ball = Ball()
                self.ball.position = Vector2(400,300)
                self.Add(self.ball)
                self.paddle1 = Paddle()
                self.paddle1.position = Vector2(100,300)
                self.Add(self.paddle1)
                self.paddle1.keyUp = KeyCode.KEY_W
                self.paddle1.keyDown = KeyCode.KEY_S
                self.paddle2 = Paddle()
                self.paddle2.position = Vector2(700,300)
                self.Add(self.paddle2)
                self.paddle2.keyUp = KeyCode.KEY_UP
                self.paddle2.keyDown = KeyCode.KEY_DOWN
        def ResetBall(self):
                self.ball.position = Vector2(400,300)
                randomDirection = Vector2.Random()
                #to make sure the ball always starts at the same
                #velocity, we set direction to 1 or -1
                #python has a weird ternary operator :D
                randomDirection.x = -1 if randomDirection.x < 0 else 1
                self.ball.velocity.y = 0.0
                self.ball.velocity.x = randomDirection.x * 200.0
        def ReceiveNote(self,message):
                if(message == "BallOffLeft" or message == "BallOffRight"):
                        self.ResetBall()
        def End(self):
                Scene.End(self)
                print "bye!"

game = Game()
game.Init()
scene = GameScene()
game.SetScene(scene)
game.Main()
