# encoding: utf-8

#
module Yast
  class Simple1Client < Client
    def main
      Yast.import "Simple1"


      Builtins.y2milestone("%1", Simple1.hello)

      nil
    end
  end
end

Yast::Simple1Client.new.main
