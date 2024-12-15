try:
    from PIL import Image
except ImportError:
    import os
    os.system('pip install pillow')
    from PIL import Image

def convert_png_to_pgm_p2(input_png, output_pgm):
    # Abra a imagem PNG e converta para escala de cinza
    img = Image.open(input_png).convert("L")
    # "L" mode converts the image to grayscale by averaging the R, G, and B values of each pixel
    # Other possible values include:
    # "1" (1-bit pixels, black and white),
    # "RGB" (3x8-bit pixels, true color),
    # "RGBA" (4x8-bit pixels, true color with transparency mask),
    # "CMYK" (4x8-bit pixels, color separation),
    # "YCbCr" (3x8-bit pixels, color video format),
    # "LAB" (3x8-bit pixels, the L*a*b color space),
    # "HSV" (3x8-bit pixels, Hue, Saturation, Value color space),
    # "I" (32-bit signed integer pixels),
    # "F" (32-bit floating point pixels)
    width, height = img.size

    # Obtenha os pixels da imagem como uma lista
    pixels = list(img.getdata())

    # Crie o arquivo PGM no formato P2
    with open(output_pgm, "w") as f:
        f.write("P2\n")  # Cabeçalho indicando formato P2
        f.write(f"{width} {height}\n")  # Dimensões da imagem
        f.write("255\n")  # Valor máximo de intensidade

        # Escreva os valores dos pixels
        for i, pixel in enumerate(pixels):
            f.write(f"{pixel} ")
            if (i + 1) % width == 0:  # Quebra de linha após cada linha da imagem
                f.write("\n")

# Exemplo de uso
convert_png_to_pgm_p2("teste.png", "teste.pgm")
