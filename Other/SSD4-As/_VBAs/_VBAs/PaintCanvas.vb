Public Class PaintCanvas
    Public Enum GraphicsSizes As Integer
        Small = 5
        Medium = 10
        Large = 25
    End Enum
    Public Enum GraphicsTools As Integer
        CirclePen = 0
    End Enum

    Public GraphicsItems As New ArrayList()
    Public GraphicsTool As GraphicsTools = GraphicsTools.CirclePen
    Public GraphicsSize As GraphicsSizes = GraphicsSizes.Medium
    Public GraphicsColor As Color = Color.Black

    Private Sub doMousePaint(ByVal e As MouseEventArgs)
        Dim objGraphicsItem As GraphicsItem
        Select Case GraphicsTool
            Case GraphicsTools.CirclePen
                Dim objGraphicCircle = New GraphicCircle()
                objGraphicCircle.SetPoint(e.X, e.Y, GraphicsSize, GraphicsColor, True)
                objGraphicsItem = objGraphicCircle
        End Select
        If objGraphicsItem IsNot Nothing Then
            GraphicsItems.Add(objGraphicsItem)
            Me.Invalidate(objGraphicsItem.rectangle)
        End If
    End Sub

    Private Sub PaintCanvas_Load(sender As Object, e As EventArgs) Handles MyBase.Load

    End Sub

    Private Sub PaintCanvas_MouseDown(sender As Object, e As MouseEventArgs) Handles MyBase.MouseDown
        If e.Button = MouseButtons.Left Then
            Me.doMousePaint(e)
        End If
    End Sub

    Private Sub PaintCanvas_MouseMove(sender As Object, e As MouseEventArgs) Handles MyBase.MouseMove
        If e.Button = MouseButtons.Left Then
            Me.doMousePaint(e)
        End If
    End Sub

    Private Sub PaintCanvas_Paint(sender As Object, e As PaintEventArgs) Handles MyBase.Paint
        For Each objGraphicsItem As GraphicsItem In GraphicsItems
            objGraphicsItem.Draw(e.Graphics)
        Next
    End Sub
    Public Sub clearCanvas()
        GraphicsItems.Clear()
        Me.Invalidate()
    End Sub
End Class
