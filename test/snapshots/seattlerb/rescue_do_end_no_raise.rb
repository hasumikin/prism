ProgramNode(0...66)(
  [],
  StatementsNode(0...66)(
    [CallNode(0...66)(
       nil,
       nil,
       IDENTIFIER(0...3)("tap"),
       nil,
       nil,
       nil,
       BlockNode(4...66)(
         [],
         nil,
         BeginNode(0...66)(
           nil,
           StatementsNode(9...15)(
             [SymbolNode(9...15)(
                SYMBOL_BEGIN(9...10)(":"),
                KEYWORD_BEGIN(10...15)("begin"),
                nil,
                "begin"
              )]
           ),
           RescueNode(16...32)(
             (16...22),
             [],
             nil,
             nil,
             StatementsNode(25...32)(
               [SymbolNode(25...32)(
                  SYMBOL_BEGIN(25...26)(":"),
                  KEYWORD_RESCUE(26...32)("rescue"),
                  nil,
                  "rescue"
                )]
             ),
             nil
           ),
           ElseNode(33...52)(
             (33...37),
             StatementsNode(40...45)(
               [SymbolNode(40...45)(
                  SYMBOL_BEGIN(40...41)(":"),
                  KEYWORD_ELSE(41...45)("else"),
                  nil,
                  "else"
                )]
             ),
             (46...52)
           ),
           EnsureNode(46...66)(
             (46...52),
             StatementsNode(55...62)(
               [SymbolNode(55...62)(
                  SYMBOL_BEGIN(55...56)(":"),
                  KEYWORD_ENSURE(56...62)("ensure"),
                  nil,
                  "ensure"
                )]
             ),
             (63...66)
           ),
           (63...66)
         ),
         (4...6),
         (63...66)
       ),
       "tap"
     )]
  )
)
