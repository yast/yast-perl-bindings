# encoding: utf-8

#
module Yast
  class Types2Client < Client
    def main
      Yast.import "Types2"
      @t = term(:MyTerm, "Hi", 42)
      @nt = term(:NestedTerm, Id(42), @t)
      Builtins.y2milestone("termloop: %1", Types2.termloop(@t))
      Builtins.y2milestone("termloop nt: %1", Types2.termloop(@nt))
      Builtins.y2milestone("termreverse: %1", Types2.termreverse(@t))

      nil
    end
  end
end

Yast::Types2Client.new.main
