# encoding: utf-8

#
module Yast
  class TypesClient < Client
    def main
      Yast.import "Types"
      Builtins.y2milestone("bool1: %1", Types.bool1)
      Builtins.y2milestone("bool2: %1", Types.bool2)

      nil
    end
  end
end

Yast::TypesClient.new.main
