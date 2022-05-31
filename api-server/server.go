package gaming

import (
	"net/http"
	"strconv"

	"github.com/gin-gonic/gin"
	"golang.org/x/time/rate"
)

func init() { gin.SetMode(gin.ReleaseMode) }

type Server struct {
	router *gin.Engine
	addr   string
	status *Status
	cmd    *Cmd
}

var lim = rate.NewLimiter(2.0, 1)

func rateLimiter(c *gin.Context) {
	if c.Request.Method != "GET" && !lim.Allow() {
		c.String(http.StatusTooManyRequests, "Too Many Requests")
		c.Abort()
	}
}

func NewServer(addr string, status *Status, cmd *Cmd) Server {
	router := gin.New()
	router.Use(rateLimiter)
	s := Server{router, addr, status, cmd}
	s.InitRoutes()
	return s
}

func (s *Server) InitRoutes() {
	s.router.GET("/angle", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Angle))
	})
	s.router.GET("/type", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Type))
	})
	s.router.GET("/delay", func(c *gin.Context) {
		c.String(http.StatusOK, strconv.Itoa(s.status.Delay))
	})

	s.router.POST("/suspend", func(c *gin.Context) {
		s.cmd.Suspend()
	})
	s.router.POST("/reset", func(c *gin.Context) {
		s.cmd.Reset()
		s.cmd.Refresh()
	})

	s.router.POST("/angle", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		s.cmd.Angle(n)
		s.cmd.Refresh()
	})
	s.router.POST("/delay", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		s.cmd.Delay(n)
		s.cmd.Refresh()
	})
	s.router.POST("/type", func(c *gin.Context) {
		v := c.PostForm("val")
		n, err := strconv.Atoi(v)
		if err != nil {
			c.Status(http.StatusBadRequest)
			return
		}
		s.cmd.Type(n)
		s.cmd.Refresh()
	})
}

func (s *Server) Run() {
	s.router.Run(s.addr)
}
