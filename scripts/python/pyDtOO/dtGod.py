#An example of a class
import logging

class dtGod:
  class __dtGod:
    def __init__(self, g, rho):
        logging.info('Create dtGod')
        self.g_ = g
        self.rho_ = rho
  instance = None

  def __init__(self, g=9.81, rho=1000.):
    if not dtGod.instance:
      dtGod.instance = dtGod.__dtGod(g, rho)
    else:
      dtGod.instance.g_ = g
      dtGod.instance.rho_ = rho

  def G(self):
      return self.instance.g_

  def Rho(self):
      return self.instance.rho_