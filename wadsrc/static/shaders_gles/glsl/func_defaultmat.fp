
void SetupMaterial(inout Material material)
{
	material.Base = ProcessTexel();
	material.Bright = texture2D(brighttexture, vTexCoord.st);
}
