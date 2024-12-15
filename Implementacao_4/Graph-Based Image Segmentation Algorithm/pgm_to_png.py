try:
    from PIL import Image
except ImportError:
    import os
    os.system('pip install pillow')
    from PIL import Image

class Pixel:
    def __init__(self, r, g, b):
        self.r = r
        self.g = g
        self.b = b

def convert_pgm_to_png(input_pgm, output_png):
    # Abra o arquivo PGM e leia os dados
    with open(input_pgm, "r") as f:
        assert f.readline().strip() == "P2"  # Verifique se o formato é P2
        width, height = map(int, f.readline().strip().split())
        max_val = int(f.readline().strip())
        pixels = []
        for line in f:
            # Remova os parênteses e divida a linha em valores
            line = line.replace("(", "").replace(")", "").replace(",", "").strip()
            # print("Line:", line)
            values = list(map(int, line.split()))
            # Crie objetos Pixel e adicione à lista
            for i in range(0, len(values), 3):
                pixel = Pixel(values[i], values[i+1], values[i+2])
                pixels.append(pixel)

    # Crie uma nova imagem RGB com as dimensões lidas
    img = Image.new("RGB", (width, height))
    # Converta a lista de objetos Pixel para uma lista de tuplas RGB
    img.putdata([(pixel.r, pixel.g, pixel.b) for pixel in pixels])
    img.save(output_png)


    

def convert_pgm_to_png2(input_pgm, output_png):
    # Abra o arquivo PGM e leia os dados
    with open(input_pgm, "r") as f:
        assert f.readline().strip() == "P2"  # Verifique se o formato é P2
        width, height = map(int, f.readline().strip().split())
        max_val = int(f.readline().strip())
        pixels = []
        for line in f:
            # Remova os parênteses e divida a linha em valores
            line = line.replace("(", "").replace(")", "").replace(",", "").strip()
            # print("Line:", line)
            values = list(map(int, line.split()))
            # Crie objetos Pixel e adicione à lista
            for i in range(0, len(values), 1):
                pixel = Pixel(values[i], values[i], values[i])
                pixels.append(pixel)

    # Crie uma nova imagem RGB com as dimensões lidas
    img = Image.new("RGB", (width, height))
    # Converta a lista de objetos Pixel para uma lista de tuplas RGB
    img.putdata([(pixel.r, pixel.g, pixel.b) for pixel in pixels])
    img.save(output_png)

# Exemplo de uso
convert_pgm_to_png("./output/converted.pgm", "./output/converted.png")