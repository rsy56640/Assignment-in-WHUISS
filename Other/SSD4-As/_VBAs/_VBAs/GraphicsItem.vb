Public MustInherit Class GraphicsItem
    Public color As Color
    Public isFilled As Boolean
    Public rectangle As Rectangle
    Public MustOverride Sub Draw(ByVal graphics As Graphics)

    Public Sub SetPoint(ByVal x As Integer, ByVal y As Integer, ByVal graphicSize As Integer, ByVal graphicsColor As Color, ByVal graphicsIsFilled As Boolean)
        rectangle = New Rectangle(x - graphicSize / 2, y - graphicSize / 2, graphicSize, graphicSize)
        color = graphicsColor
        isFilled = graphicsIsFilled

    End Sub
End Class