function changeBackground() {
    // Get the selected value
    let color = document.querySelector('input[name="colors"]:checked').value;

    // Set the color
    document.body.style.backgroundColor = color;
}