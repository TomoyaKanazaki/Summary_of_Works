//==========================================
//
//  �s�N�Z���V�F�[�_�[(SimplePS.hlsl)
//  Author : Tomoya Kanazaki
//
//==========================================

//==========================================
//  VSOutput�\���̒�`
//==========================================
struct VSOutput
{
    float4 m_pos : SV_Position; // �ʒu���W
    float4 m_col : COLOR; // ���_�J���[
};

//==========================================
//  PSOutput�\���̒�`
//==========================================
struct PSOutput
{
    float4 m_col : SV_TARGET0; // �s�N�Z���J���[
};

//==========================================
//  �s�N�Z���V�F�[�_�̃��C���G���g���\�|�C���g
//==========================================
PSOutput main(VSOutput input)
{
    PSOutput output = (PSOutput) 0;
    
    output.m_col = input.m_col;
    
    return output;
}
