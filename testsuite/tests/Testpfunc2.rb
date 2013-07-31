# encoding: utf-8

#
module Yast
  class Testpfunc2Client < Client
    def main
      Yast.import "A::Nested"

      Builtins.y2milestone("nested: %1", A::Nested.hello)

      nil
    end
  end
end

Yast::Testpfunc2Client.new.main
