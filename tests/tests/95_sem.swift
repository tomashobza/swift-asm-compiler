// SEMANTICS_ERR
// Logical error in a complex game logic function
func checkGameStatus(_playerPoints: Int, _opponentPoints: Int) -> String {
    if playerPoints > 100 && opponentPoints > 100 { // Impossible winning condition
        return "Both players win"
    } else if playerPoints > 100 {
        return "Player wins"
    } else {
        return "Game continues"
    }
}
