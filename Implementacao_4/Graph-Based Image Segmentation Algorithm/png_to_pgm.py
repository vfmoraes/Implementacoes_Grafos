try:
    from PIL import Image
except ImportError:
    import os
    os.system('pip install pillow')
    from PIL import Image
import os

class Pixel:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b
                

def convert_png_to_pgm(input_png, grapyscale_png, output_pgm):
    # Abra a imagem PNG e converta para escala de cinza
    img = Image.open(input_png).convert("L")
    width, height = img.size

    img.save(grapyscale_png)

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

def list_images(directory):
    return [f for f in os.listdir(directory) if f.endswith('.png')]

def prompt_user_to_choose_image():
    images = list_images('./images')
    print("Choose an image to convert:")
    for i, image in enumerate(images):
        print(f"{i + 1}. {image}")
    choice = int(input("Enter the number of the image: ")) - 1
    convert_png_to_pgm(f"./images/{images[choice]}", f"./output/result_gray.png", f"./output/original.pgm")



# Convert the chosen image
prompt_user_to_choose_image()


